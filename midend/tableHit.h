/*
Copyright 2017 VMware, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef MIDEND_TABLEHIT_H_
#define MIDEND_TABLEHIT_H_

#include "frontends/common/resolveReferences/resolveReferences.h"
#include "frontends/p4/typeChecking/typeChecker.h"
#include "ir/ir.h"

namespace P4 {

/**
Convert
tmp = t.apply().hit
   into
if (t.apply().hit)
   tmp = true;
else
   tmp = false;
This may be needed by some back-ends which only support hit test in conditionals
*/
class DoTableHit : public Transform, public ResolutionContext {
    TypeMap *typeMap;
    enum op_t { None, And, Or, Xor };

    const IR::Node *process(IR::BaseAssignmentStatement *statement, op_t op);

 public:
    const IR::Node *postorder(IR::BaseAssignmentStatement *statement) override {
        return process(statement, None);
    }
    const IR::Node *postorder(IR::OpAssignmentStatement *statement) override { return statement; }
    const IR::Node *postorder(IR::BAndAssign *statement) override {
        return process(statement, And);
    }
    const IR::Node *postorder(IR::BOrAssign *statement) override { return process(statement, Or); }
    const IR::Node *postorder(IR::BXorAssign *statement) override {
        return process(statement, Xor);
    }

    explicit DoTableHit(TypeMap *typeMap) : typeMap(typeMap) {
        CHECK_NULL(typeMap);
        setName("DoTableHit");
    }
};

class TableHit : public PassManager {
 public:
    explicit TableHit(TypeMap *typeMap, TypeChecking *typeChecking = nullptr) {
        if (!typeChecking) typeChecking = new TypeChecking(nullptr, typeMap);
        passes.push_back(typeChecking);
        passes.push_back(new DoTableHit(typeMap));
        setName("TableHit");
    }
};

}  // namespace P4

#endif /* MIDEND_TABLEHIT_H_ */
