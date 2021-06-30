#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }


void fir::type_checker::do_comparator_operator(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if( (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) ||
          (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else throw std::string("wrong types in comparator operator");
}

void fir::type_checker::do_equality_operator(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) ||
          (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)){
    node->type(node->left()->type());
  }
  else throw std::string("wrong types in equality operator");
}

void fir::type_checker::do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}
void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong type in unary logical expression");
  }
}
void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}
void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!(node->argument()->is_typed(cdk::TYPE_INT) || node->argument()->is_typed(cdk::TYPE_DOUBLE))){
    throw std::string("wrong type in argument of unary expression");
  }

  node->type(node->argument()->type());
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(node->left()->type());
  }
  else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){
    node->type(node->left()->type());
  }
  else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(node->left()->type());
  }
  else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)){
    node->type(node->right()->type());
  }
  else if((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) ||
          (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))){
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  }
  else throw std::string("wrong types in add");
}

void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(node->left()->type());
  }
  else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){
    node->type(node->left()->type());
  }
  else if((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) ||
          (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))){
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  }
  else if((node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) ||
  (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER))){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_POINTER));
  }
  else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)){
    auto left_type = cdk::reference_type::cast(node->left()->type())->referenced();
    auto right_type = cdk::reference_type::cast(node->right()->type())->referenced();
    while(left_type->name() == cdk::TYPE_POINTER && right_type->name() == cdk::TYPE_POINTER){
      left_type = cdk::reference_type::cast(node->left()->type())->referenced();
      right_type = cdk::reference_type::cast(node->right()->type())->referenced();
    }
    if(left_type->name() != right_type->name()){
      throw std::string("wrong type of pointer in right argument of assignment expression");
    }
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else throw std::string("wrong types in sub");
}

void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(node->left()->type());
  }
  else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){
    node->type(node->left()->type());
  }
  else if((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) ||
          (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))){
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  }
  else throw std::string("wrong types in mul");
}

void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(node->left()->type());
  }
  else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){
    node->type(node->left()->type());
  }
  else if((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) ||
          (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))){
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  }
  else throw std::string("wrong types in mul");
}

void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){
    node->type(node->left()->type());
  }
  else throw std::string("wrong types in mod");
}
void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  do_comparator_operator(node, lvl);
}
void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  do_comparator_operator(node, lvl);
}
void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  do_comparator_operator(node, lvl);
}
void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  do_comparator_operator(node, lvl);
}
void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  do_equality_operator(node, lvl);
}
void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  do_equality_operator(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<fir::symbol> symbol = _symtab.find(id);


  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);

  if (node->lvalue()->is_typed(cdk::TYPE_INT)) {
    if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else {
      throw std::string("wrong assignment to integer");
    }
  } else if (node->lvalue()->is_typed(cdk::TYPE_POINTER)) {

    if (node->rvalue()->is_typed(cdk::TYPE_POINTER)) {
        auto left_type = cdk::reference_type::cast(node->lvalue()->type())->referenced();
        auto right_type = cdk::reference_type::cast(node->rvalue()->type())->referenced();
        while(left_type->name() == cdk::TYPE_POINTER && right_type->name() == cdk::TYPE_POINTER){
          left_type = cdk::reference_type::cast(left_type)->referenced();
          right_type = cdk::reference_type::cast(right_type)->referenced();
        }
        if(left_type->name() != right_type->name() && !(left_type->name() == cdk::TYPE_DOUBLE && right_type->name() == cdk::TYPE_UNSPEC) &&
                                                      !(left_type->name() == cdk::TYPE_INT && right_type->name() == cdk::TYPE_UNSPEC)){
          throw std::string("wrong type of pointer in right argument of assignment expression");
        }
        if(right_type->name() == cdk::TYPE_UNSPEC){
          node->rvalue()->type(node->lvalue()->type());
        }
        node->type(node->rvalue()->type());
    } else {
      throw std::string("wrong assignment to pointer");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {

    if (node->rvalue()->is_typed(cdk::TYPE_DOUBLE) || node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      node->rvalue()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else {
      throw std::string("wrong assignment to real");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_STRING)) {

    if (node->rvalue()->is_typed(cdk::TYPE_STRING)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
    } else {
      throw std::string("wrong assignment to string");
    }

  } else {
    throw std::string("wrong types in assignment");
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void fir::type_checker::do_print_node(fir::print_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);

  for(size_t i = 0; i < node->argument()->size(); i++){
    cdk::expression_node *expr = (cdk::expression_node *)node->argument()->node(i);
    if(expr->is_typed(cdk::TYPE_POINTER)){
      throw std::string("cannot print pointers");
    }
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::do_read_node(fir::read_node *const node, int lvl) {
  node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);

  if(!node->condition()->is_typed(cdk::TYPE_INT)){
    throw std::string("invalid condition in while");
  }

  node->block()->accept(this, lvl + 4);
  if(node->finally()){
    node->finally()->accept(this, lvl + 4);
  }
}


//---------------------------------------------------------------------------

void fir::type_checker::do_block_node(fir::block_node *const node, int lvl) {
  //EMPTY
}

void fir::type_checker::do_prologue_node(fir::prologue_node *const node, int lvl) {
  //EMPTY
}

void fir::type_checker::do_function_call_node(fir::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = node->identifier();
  auto symbol = _symtab.find(id);
  if (symbol == nullptr) throw std::string("symbol '" + id + "' is undeclared.");
  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());

  if (node->arguments()->size() == symbol->number_of_arguments()) {
    node->arguments()->accept(this, lvl + 4);
    for (size_t ax = 0; ax < node->arguments()->size(); ax++) {
      if (node->argument(ax)->type() == symbol->argument_type(ax)) continue;
      if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && node->argument(ax)->is_typed(cdk::TYPE_INT)) continue;
      throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " of '" + id + "'.");
    }
  } else {
    throw std::string(
        "number of arguments in call (" + std::to_string(node->arguments()->size()) + ") must match declaration ("
            + std::to_string(symbol->number_of_arguments()) + ").");
  }
}

void fir::type_checker::do_function_declaration_node(fir::function_declaration_node *const node, int lvl) {
  std::string id;

  if (node->identifier() == "fir")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol for the args
  auto function = fir::make_symbol(node->qualifier(), node->type(), id, true);

  std::vector < std::shared_ptr < cdk::basic_type >> argtypes;
  for (size_t ax = 0; ax < node->arguments()->size(); ax++)
    argtypes.push_back(node->argument(ax)->type());
  function->set_argument_types(argtypes);

  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    for (size_t ax = 0; ax < node->arguments()->size(); ax++) {
      if (function->argument_type(ax) == previous->argument_type(ax)) continue;
      throw std::string("conflicting declaration for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

void fir::type_checker::do_function_definition_node(fir::function_definition_node *const node, int lvl){

    auto symbol = _symtab.find(node -> identifier());

    if(!symbol){
      std::string id;
      if (node->identifier() == "fir")
        id = "_main";
      else if (node->identifier() == "_main")
        id = "._main";
      else
        id = node->identifier();

      // remember symbol so that args know
      auto function = fir::make_symbol(node->qualifier(), node->type(), id, true);

      if(node->defaultValue()){
        node->defaultValue()->accept(this, lvl + 2);
        if(node->type() != node->defaultValue()->type() && !(node->is_typed(cdk::TYPE_DOUBLE) && node->defaultValue()->is_typed(cdk::TYPE_INT))){
          throw std::string("default value is of conflicting type '" + function->name() + "'");
        }
      }

      std::vector < std::shared_ptr < cdk::basic_type >> argtypes;
      for (size_t ax = 0; ax < node->arguments()->size(); ax++)
        argtypes.push_back(node->argument(ax)->type());
      function->set_argument_types(argtypes);

      std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
      if (previous) {
        for (size_t ax = 0; ax < node->arguments()->size(); ax++) {
          if (function->argument_type(ax) == previous->argument_type(ax)) continue;
          throw std::string("conflicting declaration for '" + function->name() + "'");
        }
      } else {
        _symtab.insert(function->name(), function);
        _parent->set_new_symbol(function);
      }
    }
    else{
      if(!symbol->isFunction()){
        throw std::string("'" + node->identifier() + "' already exists");
      }
      if((node->qualifier() != symbol->qualifier()) && !(node->qualifier() == '*' && symbol->qualifier() == '?')){
        throw std::string("conflicting types for function '" + node->identifier() + "'");
      }
      if(node->type() != symbol->type()){
        throw std::string("conflicting return type for '" + node->identifier() + "'");
      }
      if(node->defaultValue()){
        node->defaultValue()->accept(this, lvl + 2);
        if(node->type() != node->defaultValue()->type() && !(node->is_typed(cdk::TYPE_DOUBLE) && node->defaultValue()->is_typed(cdk::TYPE_INT))){
          throw std::string("default value is of conflicting type '" + symbol->name() + "'");
        }
      }
      if(node->arguments() && (symbol->number_of_arguments() > 0 )){
        if(node->arguments()->size() != symbol->number_of_arguments()){
          throw std::string("required number of arguments differ");
        }
        for(size_t i = 0; i < node->arguments()->size(); i++){
          std::shared_ptr<cdk::basic_type> res = ((cdk::typed_node *)node->arguments()->node(i))->type();
          if(res != symbol->argument_type(i)){
            throw std::string("argument types do not match");
          }
        }
      }
      _parent->set_new_symbol(symbol);
    }
}

void fir::type_checker::do_function_body_node(fir::function_body_node *const node, int lvl) {
  //EMPTY
}

void fir::type_checker::do_index_node(fir::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  std::shared_ptr < cdk::reference_type > btype;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
    btype = cdk::reference_type::cast(node->base()->type());
    if (!node->base()->is_typed(cdk::TYPE_POINTER)) throw std::string("pointer expression expected in index left-value");
  }

  node->index()->accept(this, lvl + 2);
  if (!node->index()->is_typed(cdk::TYPE_INT)) throw std::string("integer expression expected in left-value index");

  node->type(btype->referenced());
}

void fir::type_checker::do_identity_node(fir::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void fir::type_checker::do_leave_node(fir::leave_node *const node, int lvl) {
  //EMPTY
}

void fir::type_checker::do_restart_node(fir::restart_node *const node, int lvl) {
  //EMPTY
}

void fir::type_checker::do_return_node(fir::return_node *const node, int lvl) {
  //EMPTY
}

void fir::type_checker::do_sizeof_node(fir::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->expression()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_variable_declaration_node(fir::variable_declaration_node *const node, int lvl) {
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);

    if (node->is_typed(cdk::TYPE_INT)) {
      if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)){
        node->initializer()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      }
      if (!node->initializer()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type for initializer (integer expected).");
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)){
        node->initializer()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      }
      if (!node->initializer()->is_typed(cdk::TYPE_INT) && !node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
        throw std::string("wrong type for initializer (integer or double expected).");
      }
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->initializer()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for initializer (string expected).");
      }
    } else if (node->is_typed(cdk::TYPE_POINTER)) {
      if (!node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        auto in = (cdk::literal_node<int>*)node->initializer();
        if (in == nullptr || in->value() != 0) throw std::string("wrong type for initializer (pointer expected).");
      }
      auto left_type = cdk::reference_type::cast(node->type())->referenced();
      auto right_type = cdk::reference_type::cast(node->initializer()->type())->referenced();
      while(left_type->name() == cdk::TYPE_POINTER && right_type->name() == cdk::TYPE_POINTER){
        left_type = cdk::reference_type::cast(left_type)->referenced();
        right_type = cdk::reference_type::cast(right_type)->referenced();
      }
      if(left_type->name() != right_type->name() && !(left_type->name() == cdk::TYPE_DOUBLE && right_type->name() == cdk::TYPE_UNSPEC) &&
                                                    !(left_type->name() == cdk::TYPE_INT && right_type->name() == cdk::TYPE_UNSPEC)){
        throw std::string("wrong type of pointer in right argument of assignment expression");
      }
      if(right_type->name() == cdk::TYPE_UNSPEC){
        node->initializer()->type(node->type());
      }
    } else {
      throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->identifier();
  auto symbol = fir::make_symbol(node->qualifier(), node->type(), id, false);
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}

void fir::type_checker::do_nullptr_node(fir::nullptr_node *const node, int lvl){
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}

void fir::type_checker::do_address_of_node(fir::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

void fir::type_checker::do_stack_alloc_node(fir::stack_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in allocation expression");
  }

  auto mytype = cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
  node->type(mytype);
}


//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);

  if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in if condition");
  }
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->thenblock()->accept(this, lvl + 4);
  node->elseblock()->accept(this, lvl + 4);

  if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in if condition");
  }
}
