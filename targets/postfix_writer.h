#ifndef __FIR_TARGETS_POSTFIX_WRITER_H__
#define __FIR_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <sstream>
#include <set>
#include <vector>
#include <cdk/emitters/basic_postfix_emitter.h>

namespace fir {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<fir::symbol> &_symtab;
    std::shared_ptr<fir::symbol> _currentFunction;
    std::set<std::string> _functions_to_declare;
    cdk::basic_postfix_emitter &_pf;
    std::string _currentBodyRetLabel;
    std::vector<int> _whileIni, _whileEnd;
    int _lbl, _offset = 0;
    bool _inFunctionBody = false, _inFunction = false, _inFunctionArgs = false;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<fir::symbol> &symtab,
                  cdk::basic_postfix_emitter &pf) :
      basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

    void error(int lineno, std::string s) {
      std::cerr << "error: " << lineno << ": " << s << std::endl;
    }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // fir

#endif
