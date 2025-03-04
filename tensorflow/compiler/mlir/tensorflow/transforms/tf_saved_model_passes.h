/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_COMPILER_MLIR_TENSORFLOW_TRANSFORMS_TF_SAVED_MODEL_PASSES_H_
#define TENSORFLOW_COMPILER_MLIR_TENSORFLOW_TRANSFORMS_TF_SAVED_MODEL_PASSES_H_

#include <memory>

#include "mlir/Pass/Pass.h"  // from @llvm-project
#include "tensorflow/core/public/session.h"

namespace mlir {
namespace tf_saved_model {

// Creates a pass that optimizes tf_saved_model.global_tensor ops.
std::unique_ptr<OperationPass<ModuleOp>> CreateOptimizeGlobalTensorsPass();

// Creates a pass that freezes tf_saved_model.global_tensor ops.
std::unique_ptr<OperationPass<ModuleOp>> CreateFreezeGlobalTensorsPass(
    bool allow_mutable_tensors = false);

// Creates a pass that freezes tf_saved_model.asset ops.
std::unique_ptr<OperationPass<ModuleOp>> CreateFreezeAssetsPass(
    std::string saved_model_dir = "");

// Creates as pass that removes variables in the session initializer.
// This job is required with lifting variable passes. Originally, the session
// initializer function does assigning variables. However, the read-only
// variable assignments will be done via lifting variables pass by converting
// the read-only variables to constant ops, instead. This pass removes the
// redundant operations. This pass should be located in front of the pass for
// lifting read-only variables.
std::unique_ptr<OperationPass<ModuleOp>>
CreateRemoveVariablesInSessionInitializerPass();

// Creates as pass that creates GlobalTensorOp for each variable from function
// arguments and converts the function arguments to the corresponding saved
// model arguments.
std::unique_ptr<OperationPass<ModuleOp>> CreateLiftVariablesPass(
    ::tensorflow::Session* session);

// Creates a pass that removes duplicate 'tf_saved_model.bound_input' bindings.
std::unique_ptr<OperationPass<FuncOp>> CreateDedupBoundInputBindingPass();

// Creates a pass that marks variables whether they are initialized or not.
std::unique_ptr<OperationPass<FuncOp>> CreateMarkInitializedVariablesPass(
    ::tensorflow::Session* session);

// Creates a pass that initializes all variables in Session Init function
// for all variables in 'session'.
std::unique_ptr<OperationPass<ModuleOp>>
CreateInitializeVariablesInSessionInitializerPass(tensorflow::Session* session);

// Create a test pass for the above with a "fake" session, for lit testing.
std::unique_ptr<OperationPass<ModuleOp>>
CreateInitializeVariablesInSessionInitializerTestPass();

#define GEN_PASS_REGISTRATION
#include "tensorflow/compiler/mlir/tensorflow/transforms/tf_savedmodel_passes.h.inc"

}  // namespace tf_saved_model

}  // namespace mlir

#endif  // TENSORFLOW_COMPILER_MLIR_TENSORFLOW_TRANSFORMS_TF_SAVED_MODEL_PASSES_H_
