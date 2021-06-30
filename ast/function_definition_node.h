#ifndef __FIR_AST_FUNCTION_DEFINITION_NODE_H__
#define __FIR_AST_FUNCTION_DEFINITION_NODE_H__

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/block_node.h"

namespace fir {

  //!
  //! Class for describing function definitions.
  //! <pre>
  //! declaration: type qualifier id '(' args ')' block
  //!            {
  //!              $$ = new og::function_definition(LINE, $1, $2, $3, $5, $7);
  //!            }
  //! </pre>
  //!
  class function_definition_node: public cdk::typed_node {
    int _qualifier;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    fir::function_body_node *_function_body;
    cdk::expression_node *_defaultValue;

  public:
    function_definition_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *arguments,
                             fir::function_body_node *function_body) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _function_body(function_body) {
      type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
    }

    function_definition_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> funType, const std::string &identifier,
                             cdk::sequence_node *arguments, fir::function_body_node *function_body) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _function_body(function_body), _defaultValue(nullptr) {
      type(funType);
    }

    function_definition_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> funType, const std::string &identifier,
                             cdk::sequence_node *arguments, fir::function_body_node *function_body, cdk::expression_node *defaultValue) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _function_body(function_body), _defaultValue(defaultValue) {
      type(funType);
    }

  public:
    int qualifier() {
      return _qualifier;
    }
    const std::string& identifier() const {
      return _identifier;
    }
    cdk::sequence_node* arguments() {
      return _arguments;
    }
    cdk::typed_node* argument(size_t ax) {
      return dynamic_cast<cdk::typed_node*>(_arguments->node(ax));
    }
    cdk::expression_node* defaultValue(){
      return _defaultValue;
    }
    fir::function_body_node* function_body() {
      return _function_body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // fir

#endif
