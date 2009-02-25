/*
 * Copyright 2007, 2008 Timo Puronen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SPECIFICATION_H_
#define SPECIFICATION_H_

#include "Runnable.h"
#include "Reporter.h"
#include "Matcher.h"
#include "ShouldType.h"
#include "ContextHolder.h"
#include "NamedFunctor.h"
#include "InvokingType.h"
#include "TypeHasStreamingOperator.h"
#include <vector>
#include <sstream>

/**
 * MSVC++ does not support not keyword.
 */
#ifdef _WIN32
#define not !
#endif

namespace CppSpec {

/**
 * Specification base class.
 * @param Context the class under test.
 * @param Derived the concrete specification class as in curiously recurring template pattern.
 */
template <class Context, class Derived>
class Specification : public Runnable, ContextHolder<Context> {
public:
	explicit Specification() : should(*this), contextPtr(NULL), reporter(), behaviors(), name() {
	}

	virtual ~Specification() {
		static_cast<Derived*>(this)->destroyContext();
	}

	/**
	 * Create the context object. Can be overridden in derived specification class.
	 */
	Context* createContext() {
		return new Context;
	}

	/**
	 * Destroy the context object. Can be overridden in derived specification class.
	 */
	void destroyContext() {
		delete contextPtr;
		contextPtr = NULL;
	}

public: // from ContextHolder
	Context& getContext() {
		return *contextPtr;
	}

private:
	template<bool v>
	struct CheckIf {
		enum { value = v };
	};

public: // Expectations, these are used through specify-macro
	/**
	 * Specify that condition must be fullfilled.
	 */
	void specifyImpl(const std::string& file, int line, bool condition) {
		if(condition) {
			reporter->behaviorSucceeded();
		} else {
			reporter->behaviorFailed(file, line, "Expected true, but was false");
		}
	}

	/**
	 * Specify that actual and expected must equal.
	 */
	template<class T>
	void specifyImpl(const std::string& file, int line, const T& actual, const Expectation<T>& expected) {
		if(expected.equals(actual)) {
			reporter->behaviorSucceeded();
		} else {
			std::stringstream message;
			writeInequalMessageToStream(message, actual, expected, CheckIf<TypeHasStreamingOperator<T>::result>());
			reporter->behaviorFailed(file, line, message.str());
		}
	}

	/**
	 * Specify that actual and expected must equal.
	 */
	template<class T, class U>
	void specifyImpl(const std::string& file, int line, const T& actual, const Expectation<U>& expected) {
		if(expected.equals(actual)) {
			reporter->behaviorSucceeded();
		} else {
			std::stringstream message;
			writeInequalMessageToStream(message, actual, expected, CheckIf<TypeHasStreamingOperator<T>::result && TypeHasStreamingOperator<U>::result>());
			reporter->behaviorFailed(file, line, message.str());
		}
	}

	void specifyImpl(const std::string& file, int line, const std::string& text, const Matcher& matcher) {
		if(matcher(text)) {
			reporter->behaviorSucceeded();
		}
		else {
			std::stringstream message;
			if(matcher.getMode() == Matcher::Search) {
				message << matcher.getExpression() << " was not found in " << text;
			} else {
				message << matcher.getExpression() << " did not match " << text;
			}
			reporter->behaviorFailed(file, line, message.str());
		}
	}

	template<class R>
	InvokingType<R, Context> invoking(R (Context::*function)()) {
		InvokingType<R, Context> functor;
		functor.setFunctor(&getContext(), function);
		return functor;
	}

	template<class R, class Arg1>
	InvokingType<R, Context> invoking(R (Context::*function)(Arg1), Arg1 arg1) {
		InvokingType<R, Context> functor;
		functor.setFunctor(&getContext(), function, arg1);
		return functor;
	}

	template<class R, class Arg1, class Arg2>
	InvokingType<R, Context> invoking(R (Context::*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) {
		InvokingType<R, Context> functor;
		functor.setFunctor(&getContext(), function, arg1, arg2);
		return functor;
	}

	template<class R, class Arg1, class Arg2, class Arg3>
	InvokingType<R, Context> invoking(R (Context::*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3) {
		InvokingType<R, Context> functor;
		functor.setFunctor(&getContext(), function, arg1, arg2, arg3);
		return functor;
	}

	template<class R, class Arg1, class Arg2, class Arg3, class Arg4>
	InvokingType<R, Context> invoking(R (Context::*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
		InvokingType<R, Context> functor;
		functor.setFunctor(&getContext(), function, arg1, arg2, arg3, arg4);
		return functor;
	}


public: // from Runnable
	void operator()() {
		reporter->specificationStarted(*this);
		const int count(behaviors.size());
		for(int i = 0; i < count; ++i) {
			contextPtr = static_cast<Derived*>(this)->createContext();
			NamedFunctor& behavior = *(behaviors[i]);
			reporter->behaviorStarted(behavior.getName());
			try {
				behavior();
			}
			catch(std::exception& e) {
				std::stringstream msg;
				msg << "An exception " << e.what() << " occured in " << behavior.getName();
				reporter->behaviorFailed("", 0, msg.str());
			}
			catch(...) {
				std::stringstream msg;
				msg << "An exception occured in " << behavior.getName();
				reporter->behaviorFailed("", 0, msg.str());
			}
			static_cast<Derived*>(this)->destroyContext();
		}
		reporter->specificationEnded(name);
	}

	void setName(const std::string& name) {
		this->name = name;
	}

	const std::string& getName() const {
		return name;
	}

	void setReporter(Reporter* reporter) {
		this->reporter = reporter;
	}

	unsigned int getBehaviorCount() const {
		return behaviors.size();
	}

public: // Vocabulary
	Context& context() {
		return *contextPtr;
	}

	ShouldType<Context> should;

private:
	template<class T>
	void writeInequalMessageToStream(std::ostream& stream, const T& actual, const Expectation<T>& expected, CheckIf<true>) {
		stream << std::boolalpha << "expected " << expected() << ", but was " << actual;
	}

	template<class T, class U>
	void writeInequalMessageToStream(std::ostream& stream, const T& actual, const Expectation<U>& expected, CheckIf<true>) {
		stream << std::boolalpha << "expected " << expected() << ", but was " << actual;
	}

	template<class T>
	void writeInequalMessageToStream(std::ostream& stream, const T& actual, const Expectation<T>& expected, CheckIf<false>) {
		stream << "actual was not expected";
	}

protected:
	Context* contextPtr;

private:
	Reporter* reporter;

protected:
	typedef std::vector<NamedFunctor*> BehaviorList;
	BehaviorList behaviors;
	std::string name;

private:
	Specification(const Specification&);
	Specification& operator=(const Specification&);
};

}
#endif /*SPECIFICATION_H_*/
