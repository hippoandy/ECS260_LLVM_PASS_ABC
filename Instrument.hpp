#ifndef INSTRUMENT_GUARD
#define INSTRUMENT_GUARD 1

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/IRBuilder.h>

namespace llvm {
	class Type;
}

using namespace std;
using namespace llvm;

class Instrument : public ModulePass {
public:
  Instrument() : ModulePass(ID) {}

  virtual bool doInitialization(Module &module);

  bool runOnFunction(Module &module, Function &function);

  virtual void getAnalysisUsage(AnalysisUsage &AU) const;

  virtual bool runOnModule(Module &module);

  static char ID; // Pass identification, replacement for typeid

private:

};
#endif
