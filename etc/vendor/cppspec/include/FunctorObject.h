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

#ifndef FUNCTOROBJECT_H_
#define FUNCTOROBJECT_H_

#include "Functor.h"

namespace CppSpec {

template<class ReturnType, class Target>
class FunctorObject0 : public Functor {
public:
	explicit FunctorObject0(Target* target, ReturnType (Target::*Method)()) : target(target), method(Method) {
	}

	FunctorObject0(const FunctorObject0& that) : target(that.target), method(that.method) {
	}

	void operator()() {
		(target->*method)();
	}

	Functor* clone() {
		return new FunctorObject0(*this);
	}

private:
	Target* target;
	ReturnType (Target::*method)();
};

template<class ReturnType, class Target, class Arg1>
class FunctorObject1 : public Functor {
public:
	explicit FunctorObject1(Target* target, ReturnType (Target::*Method)(Arg1), Arg1 arg1) : target(target), method(Method), arg1(arg1) {
	}

	FunctorObject1(const FunctorObject1& that) : target(that.target), method(that.method), arg1(that.arg1) {
	}

	void operator()() {
		(target->*method)(arg1);
	}

	Functor* clone() {
		return new FunctorObject1(*this);
	}

private:
	Target* target;
	ReturnType (Target::*method)(Arg1);
	Arg1 arg1;
};

template<class ReturnType, class Target, class Arg1, class Arg2>
class FunctorObject2 : public Functor {
public:
	explicit FunctorObject2(Target* target, ReturnType (Target::*Method)(Arg1, Arg2), Arg1 arg1, Arg2 arg2)
	: target(target), method(Method), arg1(arg1), arg2(arg2) {
	}

	FunctorObject2(const FunctorObject2& that) : target(that.target), method(that.method), arg1(that.arg1), arg2(that.arg2) {
	}

	void operator()() {
		(target->*method)(arg1, arg2);
	}

	Functor* clone() {
		return new FunctorObject2(*this);
	}

private:
	Target* target;
	ReturnType (Target::*method)(Arg1, Arg2);
	Arg1 arg1;
	Arg2 arg2;
};

template<class ReturnType, class Target, class Arg1, class Arg2, class Arg3>
class FunctorObject3 : public Functor {
public:
	explicit FunctorObject3(Target* target, ReturnType (Target::*Method)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3)
	: target(target), method(Method), arg1(arg1), arg2(arg2), arg3(arg3) {
	}

	FunctorObject3(const FunctorObject3& that) : target(that.target), method(that.method), arg1(that.arg1), arg2(that.arg2), arg3(that.arg3) {
	}

	void operator()() {
		(target->*method)(arg1, arg2, arg3);
	}

	Functor* clone() {
		return new FunctorObject3(*this);
	}

private:
	Target* target;
	ReturnType (Target::*method)(Arg1, Arg2, Arg3);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
};

template<class ReturnType, class Target, class Arg1, class Arg2, class Arg3, class Arg4>
class FunctorObject4 : public Functor {
public:
	explicit FunctorObject4(Target* target, ReturnType (Target::*Method)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
	: target(target), method(Method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {
	}

	FunctorObject4(const FunctorObject4& that) : target(that.target), method(that.method), arg1(that.arg1), arg2(that.arg2), arg3(that.arg3), arg4(that.arg4) {
	}

	void operator()() {
		(target->*method)(arg1, arg2, arg3, arg4);
	}

	Functor* clone() {
		return new FunctorObject4(*this);
	}

private:
	Target* target;
	ReturnType (Target::*method)(Arg1, Arg2, Arg3, Arg4);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
};

template<class ReturnType, class Target>
FunctorObject0<ReturnType, Target>* createFunctor(Target* target, ReturnType (Target::*Method)()) {
	return new FunctorObject0<ReturnType, Target>(target, Method);
}

template<class ReturnType, class Target, class Arg1>
FunctorObject1<ReturnType, Target, Arg1>* createFunctor(Target* target, ReturnType (Target::*Method)(Arg1), Arg1 arg1) {
	return new FunctorObject1<ReturnType, Target, Arg1>(target, Method, arg1);
}

template<class ReturnType, class Target, class Arg1, class Arg2>
FunctorObject2<ReturnType, Target, Arg1, Arg2>* createFunctor(Target* target, ReturnType (Target::*Method)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) {
	return new FunctorObject2<ReturnType, Target, Arg1, Arg2>(target, Method, arg1, arg2);
}

template<class ReturnType, class Target, class Arg1, class Arg2, class Arg3>
FunctorObject3<ReturnType, Target, Arg1, Arg2, Arg3>* createFunctor(Target* target, ReturnType (Target::*Method)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3) {
	return new FunctorObject3<ReturnType, Target, Arg1, Arg2, Arg3>(target, Method, arg1, arg2, arg3);
}

template<class ReturnType, class Target, class Arg1, class Arg2, class Arg3, class Arg4>
FunctorObject4<ReturnType, Target, Arg1, Arg2, Arg3, Arg4>* createFunctor(Target* target, ReturnType (Target::*Method)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
	return new FunctorObject4<ReturnType, Target, Arg1, Arg2, Arg3, Arg4>(target, Method, arg1, arg2, arg3, arg4);
}

}
#endif /* FUNCTOROBJECT_H_ */
