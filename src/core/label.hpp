#ifndef __LABEL_H
#define __LABEL_H
#include "FRObj.hpp"

/**
 * @class Label
 * LLVMのLabelを表すクラス
 */
class Label : public FRObj {
public:
  Label() : FRObj("undefined", "asm_undefined") {}
};
#endif