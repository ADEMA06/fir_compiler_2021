#ifndef __FIR_AST_IDENTITY_NODE_H__
#define __FIR_AST_IDENTITY_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace fir {

  class identity_node: public cdk::unary_operation_node {

  public:
    identity_node(int lineno, cdk::expression_node *arg) :
        cdk::unary_operation_node(lineno, arg) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // fir

#endif
