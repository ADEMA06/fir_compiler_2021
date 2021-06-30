#ifndef __FIR_TARGETS_SYMBOL_H__
#define __FIR_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace fir {

  class symbol {

    std::string _name; // identifier

    int _qualifier; // qualifiers: public, forward, "private" (i.e., none)
    std::shared_ptr<cdk::basic_type> _type; // type (type id + type size)
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;
    int _offset = 0; // 0 (zero) means global variable/function
    bool _function; // false for variables

    public:
    symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool function) :
        _name(name), _qualifier(qualifier), _type(type), _function(function){
    }

    ~symbol() {
    }

    const std::string& name() const {
      return _name;
    }

    int qualifier() const {
      return _qualifier;
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }

    const std::string& identifier() const {
      return name();
    }
    int offset() const {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool isFunction() const {
      return _function;
    }

    bool global() const {
      return _offset == 0;
    }
    bool isVariable() const {
      return !_function;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }

  };

  inline auto make_symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool function) {
    return std::make_shared<symbol>(qualifier, type, name, function);
  }

} // fir

#endif
