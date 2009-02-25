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

#ifndef SHOULDTYPE_H_
#define SHOULDTYPE_H_

#include "BeType.h"
#include "Expectation.h"
#include "Matcher.h"

#include <algorithm>
#include <string>

namespace CppSpec {

template<typename Context>
class ShouldType {
public:
	explicit ShouldType(ContextHolder<Context>& creator)
		: be(creator), have(*this), contextHolder(creator)
	{
	}

public: // Interface
	/**
	 * Specify that context must implement the given interface.
	 */
	template<class Base>
	bool implement() {
		Base* base = dynamic_cast<Base*>(&(contextHolder.getContext()));
		return base != NULL;
	}

	template<typename T>
	bool element(const T& t) {
		return std::find(contextHolder.getContext().begin(), contextHolder.getContext().end(), t) != contextHolder.getContext().end();
	}

	template<class Iterator>
	bool elements(Iterator begin, Iterator end) {
		return std::search(contextHolder.getContext().begin(), contextHolder.getContext().end(), begin, end) != contextHolder.getContext().end();
	}

	template<typename T>
	Expectation<T> equal(const T& t) {
		return Expectation<T>(t);
	}

	Matcher contain(const std::string& regex) {
		return Matcher(regex, Matcher::Search);
	}

	Matcher match(const std::string& regex) {
		return Matcher(regex, Matcher::Match);
	}

public: // Members
	BeType<Context> be;
	ShouldType<Context>& have;

private:
	ContextHolder<Context>& contextHolder;
};

}

#endif /*SHOULDTYPE_H_*/
