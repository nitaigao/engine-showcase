/*
 * Copyright 2008 Timo Puronen
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

#ifndef INVOKINGTYPE_H_
#define INVOKINGTYPE_H_

#include "FunctorObject.h"

namespace CppSpec {

template<class ReturnType, class Target>
class InvokingType {
public:
	explicit InvokingType() : should(*this), raise(*this), functor(NULL) {
	}

	virtual ~InvokingType() {
		delete functor;
	}

	InvokingType(const InvokingType& that) : should(*this), raise(*this) {
		functor = that.functor->clone();
	}

	void setFunctor(Target* target, ReturnType (Target::*method)()) {
		functor = createFunctor(target, method);
	}

	template<class Arg1>
	void setFunctor(Target* target, ReturnType (Target::*method)(Arg1), Arg1 arg1) {
		functor = createFunctor(target, method, arg1);
	}

	template<class Arg1, class Arg2>
	void setFunctor(Target* target, ReturnType (Target::*method)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) {
		functor = createFunctor(target, method, arg1, arg2);
	}

	template<class Arg1, class Arg2, class Arg3>
	void setFunctor(Target* target, ReturnType (Target::*method)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3) {
		functor = createFunctor(target, method, arg1, arg2, arg3);
	}

	template<class Arg1, class Arg2, class Arg3, class Arg4>
	void setFunctor(Target* target, ReturnType (Target::*method)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
		functor = createFunctor(target, method, arg1, arg2, arg3, arg4);
	}

	template<class T>
	bool exception() {
		try {
			(*functor)();
		}
		catch(T&) {
			return true;
		}
		catch(...) {
		}
		return false;
	}

	template<typename T>
	bool exception(T& expected) {
		try {
			(*functor)();
		}
		catch(T& occured) {
			return occured == expected;
		}
		catch(...) {
		}
		return false;
	}

	template<typename T>
	bool exception(const T& e) {
		return exception(const_cast<T&>(e));
	}

public:
	InvokingType& should;
	InvokingType& raise;

private:
	Functor* functor;
};

}

#endif /* INVOKINGTYPE_H_ */
