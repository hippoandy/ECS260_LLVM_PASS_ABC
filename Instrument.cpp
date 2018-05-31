#include "Instrument.hpp"

#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <iostream>

int instcount=0;
bool Instrument::doInitialization(Module &module) {
  return false;
}

bool Instrument::runOnModule(Module &module) {
  for(Module::iterator f = module.begin(), fe = module.end(); f != fe; f++) {
    runOnFunction(module, *f);
  }
  errs() << "  --" << instcount << " instruction(s) instrumented\n";
  return true;
}

static void createCallInst(Module &module, Instruction *instruction) {
  // prepare for STEP-5
  IRBuilder<> builder( instruction );

  /***
  ** STEP-2: Retrieving the Array Size
  **/
  unsigned array_size = 0;
  GetElementPtrInst* getElePtrInst = dyn_cast<GetElementPtrInst>( instruction );
  Type* elementType = cast<PointerType>( getElePtrInst->getPointerOperandType() )->getElementType();
  // make sure the pointer is an array pointer
  if (elementType->isArrayTy()) {
    ArrayType *Aty = dyn_cast<ArrayType>(elementType);
    array_size = (unsigned) Aty->getNumElements();
  }
  ConstantInt* arrSize = builder.getInt64( (uint64_t) array_size );

  /***
  ** STEP-3: Retrieving the Index
  **/
  ConstantInt* conInst;
  Value* arrIndex;
  // the sample instruction looks like this:
  //   %9 = getelementptr inbounds [10 x i32], [10 x i32]* %numbers, i64 0, i64 %8, !dbg !30
  //   "i64 0"  locates at getOperand(1)
  //   "i64 %8" locates at getOperand(2), and this is the array index
  //   the %8 refer to the previous instruction if it is a dynamic index
  //   otherwise, it is a integer value
  if ( isa<Instruction>( getElePtrInst->getOperand(2) ) ) {
    // to deal with dynamic array index
    arrIndex = dyn_cast<Value>( getElePtrInst->getOperand(2) );
  }
  else {
    // to deal with static array index like: arr[5]
    conInst = dyn_cast<ConstantInt>(getElePtrInst->getOperand(2));
    arrIndex = builder.getInt64( (uint64_t) conInst->getSExtValue() );
  }

  /***
  ** STEP-4: Retrieving the Code Line Number
  **/
  // this step requried the '-g' option when compiling, which is already enabled
  Function *calledFunct = dyn_cast<Function>( instruction->getFunction() );
  // get the metadata create by clang through '-g' option
  // the "dbg" flag should refer to the debug metadata of an instruction
  MDNode *meta = instruction->getMetadata("dbg");
  DILocation *dILoc = dyn_cast<DILocation>( meta );
  //errs() << dILoc->getLine() << "\n";
  //errs() << dILoc->getFilename() << "\n";
  //errs() << dILoc->getDirectory() << "\n";
  ConstantInt* lineNum = builder.getInt64( (uint64_t) dILoc->getLine() );

  /***
  ** STEP-5: Creating and Passing the Arguments
  ** Pack the retrieved arguments to pass to the Out-of-Bounds checking function
  ** and modify the check.c file accordingly.
  ** Note: feel free to not use the code that is commented out below
  **/
  std::vector<Value *> args;
  args.push_back( arrSize );
  args.push_back( arrIndex );
  args.push_back( lineNum );

  /***
  ** STEP-6: Extra Credit
  ** to print out the filename in the debug information
  ** concatenate the filename pointer to the args vector
  ***/
  args.push_back( builder.CreateGlobalStringPtr( dILoc->getFilename() ) );


  Function * callee = module.getFunction("OutofBoundsError");
  if (callee) {
    // this call will help arrange the function input
    CallInst::Create(callee, args, "", instruction);
  }

  return;
}


bool Instrument::runOnFunction(Module &module, Function &function) {

  if (! function.isDeclaration()) {
    for(Function::iterator b = function.begin(), be = function.end(); b != be; b++) {
      for(BasicBlock::iterator i = b->begin(), ie = b->end(); i != ie; i++) {
        /***
    	** STEP-1: Instrumenting the Array Access Instructions
	** Reimplement the following if-condition statement
    	** to instrument the Out-of-Bounds check function after each array access instruction.
	***/
        Instruction* inst = dyn_cast<Instruction>(i);
//        errs() << "  Instruction: " << *inst << "\n";

//        if ( isa<AllocaInst>( inst ) ) {
//          AllocaInst* allocaInst = dyn_cast<AllocaInst>( inst );
//          Type* elementType = allocaInst->getType()->getElementType();
//          if (elementType->isArrayTy()) {
//            errs() << "  Get!!!!!!! " << "\n";
//            DataLayout dataLayout = module.getDataLayout();
//            uint64_t size = dataLayout.getTypeAllocSize( elementType );
//            ArrayType *Aty = dyn_cast<ArrayType>(elementType);
//            uint64_t size = Aty->getNumElements();
//            errs() << "  Size: " << size << "\n";
//          }
//        }

        if ( isa<LoadInst>( inst ) ) {}

        //if (inst&&0) {
        // the GetElementPtrInst should be the array access instruction
        if ( isa<GetElementPtrInst>( inst ) ) {
	  createCallInst(module, inst);

          /* Keep the following code to count and print the instrumented instructions. */
          errs() << "  Instruction: " << *inst << "\n";
          instcount++;
	}
      }
    }
  }
  return true;

}

void Instrument::getAnalysisUsage(AnalysisUsage &AU __attribute__((unused))) const {
  return;
}

char Instrument::ID = 0;
static const RegisterPass<Instrument> registration("instrument", "Instruments array accesses for out-of-bound checks");
