@echo off
soul2html -v calc.token
soul2html -v calc.scmlg.1
cm2html -v calc.token.cm.cm
soul2html -v calc.expr
soul2html -v calc.lexer
soul2html -v calc.scmlg.2
cm2html -v calc.cmp.1.cm
soul2html -v calc.1.parser
cm2html -v ast.1.cm
soul2html -v calc.scmpg
cm2html -v calc.1.parser.skeleton.cm
cm2html -v ast.2.cm
soul2html -v calc.2.parser
cm2html -v ast.3.cm
soul2html -v calc.3.parser
soul2html -v calc.4.parser
cm2html -v ast.4.cm
soul2html -v calc.5.parser
cm2html -v ast.5.cm
soul2html -v calc.6.parser
cm2html -v evaluate.cm
cm2html -v main.cm
cm2html -v calc.cmp.cm

