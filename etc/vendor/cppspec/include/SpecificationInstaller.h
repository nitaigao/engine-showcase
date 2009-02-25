/*
 * Copyright 2007 Timo Puronen
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

#ifndef SPECIFICATIONINSTALLER_H_
#define SPECIFICATIONINSTALLER_H_

#include "SpecificationRegistry.h"
#include "Runnable.h"
#include "CppSpecConfig.h"

namespace CppSpec {

class CppSpec_EXPORT SpecificationInstaller {
public:
	explicit SpecificationInstaller(Runnable* specification, const char* name);
	
private:
	SpecificationInstaller(const SpecificationInstaller&);
};

}

#endif /*SPECIFICATIONINSTALLER_H_*/
