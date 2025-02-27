#ifndef COMMON_H
#define COMMON_H

#include <llvm/IR/Module.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/IR/DebugInfo.h>

#include <unistd.h>
#include <bitset>
#include <chrono>

using namespace llvm;
using namespace std;

#define OP llvm::errs()

extern map<Type *, string> TypeToTNameMap;
extern const DataLayout *CurrentLayout;

//
// Common functions
//

string getFileName(DILocation *Loc,
                   DISubprogram *SP = NULL);

bool isConstant(Value *V);

string getSourceLine(string fn_str, unsigned lineno);

string getSourceFuncName(Instruction *I);

StringRef getCalledFuncName(Instruction *I);

string extractMacro(string, Instruction *I);

DILocation *getSourceLocation(Instruction *I);

void printSourceCodeInfo(Value *V);

void printSourceCodeInfo(Function *F);

string getMacroInfo(Value *V);

void getSourceCodeInfo(Value *V, string &file,
                       unsigned &line);

Argument *getArgByNo(Function *F, int8_t ArgNo);

size_t funcHash(Function *F, bool withName = true);

size_t callHash(CallInst *CI);

size_t typeHash(Type *Ty);

size_t typeIdxHash(Type *Ty, int Idx = -1);

size_t hashIdxHash(size_t Hs, int Idx = -1);

string HandleSimpleTy(Type *Ty);

string expand_struct(StructType *STy);


void getSourceCodeLine(Value *V, string &line);

//
// Common data structures
//
class ModuleOracle {
public:
    ModuleOracle(Module &m) :
            dl(m.getDataLayout()),
            tli(TargetLibraryInfoImpl(Triple(Twine(m.getTargetTriple())))) {}

    ~ModuleOracle() {}

    // Getter
    const DataLayout &getDataLayout() {
        return dl;
    }

    TargetLibraryInfo &getTargetLibraryInfo() {
        return tli;
    }

    // Data layout
    uint64_t getBits() {
        return Bits;
    }

    uint64_t getPointerWidth() {
        return dl.getPointerSizeInBits();
    }

    uint64_t getPointerSize() {
        return dl.getPointerSize();
    }

    uint64_t getTypeSize(Type *ty) {
        return dl.getTypeAllocSize(ty);
    }

    uint64_t getTypeWidth(Type *ty) {
        return dl.getTypeSizeInBits(ty);
    }

    uint64_t getTypeOffset(Type *type, unsigned idx) {
        assert(isa<StructType>(type));
        return dl.getStructLayout(cast<StructType>(type))
                ->getElementOffset(idx);
    }

    bool isReintPointerType(Type *ty) {
        return (ty->isPointerTy() ||
                (ty->isIntegerTy() &&
                 ty->getIntegerBitWidth() == getPointerWidth()));
    }

protected:
    // Info provide
    const DataLayout &dl;
    TargetLibraryInfo tli;

    // Consts
    const uint64_t Bits = 8;
};

class Helper {
public:
    // LLVM value
    static string getValueName(Value *v) {
        if (!v->hasName()) {
            return to_string(reinterpret_cast<uintptr_t>(v));
        } else {
            return v->getName().str();
        }
    }

    static string getValueType(Value *v) {
        if (Instruction *inst = dyn_cast<Instruction>(v)) {
            return string(inst->getOpcodeName());
        } else {
            return string("value " + to_string(v->getValueID()));
        }
    }

    static string getValueRepr(Value *v) {
        string str;
        raw_string_ostream stm(str);

        v->print(stm);
        stm.flush();

        return str;
    }

    // String conversion
    static void convertDotInName(string &name) {
        replace(name.begin(), name.end(), '.', '_');
    }
};

class Dumper {
public:
    Dumper() {}

    ~Dumper() {}

    // LLVM value
    void valueName(Value *val) {
        errs() << Helper::getValueName(val) << "\n";
    }

    void typedValue(Value *val) {
        errs() << "[" << Helper::getValueType(val) << "]"
               << Helper::getValueRepr(val)
               << "\n";
    }
};

extern Dumper DUMP;

class SecurityCheck {
public:
    SecurityCheck(Value *sk, Value *br) : SCheck(sk), SCBranch(br) {
        auto I = dyn_cast<Instruction>(SCheck);
        if (!I)
            return;

        MDNode *N = I->getMetadata("dbg");
        if (!N)
            return;

        DILocation *Loc = dyn_cast<DILocation>(N);
        if (!Loc || Loc->getLine() < 1)
            return;

        SCheckFileName = Loc->getFilename().str();
        SCheckLineNo = Loc->getLine();
    }

    ~SecurityCheck() {
    }

    Value *getSCheck() { return SCheck; }

    Value *getSCBranch() { return SCBranch; }

    string getSCheckFileName() { return SCheckFileName; }

    unsigned getSCheckLineNo() { return SCheckLineNo; }

    friend bool operator<(const SecurityCheck &SC1, const SecurityCheck &SC2) {
        return (SC1.SCheck < SC2.SCheck);
    }

private:
    Value *SCheck;          /* Security check of this critical variable */
    Value *SCBranch;        /* Branch associated to the check */
    string SCheckFileName; /* Source file name of security check */
    unsigned SCheckLineNo;  /* Line number of security check */
};

#endif
