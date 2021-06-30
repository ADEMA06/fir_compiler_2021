#ifndef __FIR_AST_FUNCTION_BODY_NODE_H__
#define __FIR_AST_FUNCTION_BODY_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace fir {

  class function_body_node: public cdk::basic_node {
    fir::prologue_node *_prologue;
    fir::block_node *_block;
    fir::block_node *_epilogue;

  public:
    function_body_node(int lineno, fir::prologue_node *prologue, fir::block_node *block, fir::block_node *epilogue) :
        cdk::basic_node(lineno), _prologue(prologue), _block(block), _epilogue(epilogue) {
    }

  public:
    fir::prologue_node* prologue() {
      return _prologue;
    }

    fir::block_node* block() {
      return _block;
    }

    fir::block_node* epilogue() {
      return _epilogue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_body_node(this, level);
    }

  };

} // fir

#endif
