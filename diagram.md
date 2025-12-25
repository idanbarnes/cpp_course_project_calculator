
classDiagram
    direction LR

    class Token {
        +type : TokenType
        +lexeme : std::string
    }

    class Lexer {
        -text : std::string
        -pos : size_t
        +Lexer(input : std::string)
        +nextToken() : Token
        +reset() : void
        -skipWhitespace() : void
    }

    class Parser {
        -lex : Lexer&
        -current : Token
        +Parser(lexer : Lexer&)
        +parseExpression() : unique_ptr<ASTNode>
        -advance() : void
        -expect(t : TokenType) : void
        -parseTerm() : unique_ptr<ASTNode>
        -parseFactor() : unique_ptr<ASTNode>
        -parsePrimary() : unique_ptr<ASTNode>
    }

    class ASTNode {
        <<abstract>>
    }

    class NumberNode {
        +raw : std::string
    }

    class VariableNode {
        +name : std::string
    }

    class BinaryOpNode {
        +op : char
        +left : unique_ptr<ASTNode>
        +right : unique_ptr<ASTNode>
    }

    class UnaryOpNode {
        +op : char
        +child : unique_ptr<ASTNode>
    }

    class UnaryFunctionNode {
        +func : std::string
        +arg : unique_ptr<ASTNode>
    }

    class SymbolTable {
        -table : map<string,double>
        +set(name : string, value : double) : void
        +get(name : string, outValue : double&) : bool
    }

    class Evaluator {
        -symbols : SymbolTable&
        +Evaluator(st : SymbolTable&)
        +evaluate(root : ASTNode*) : double
        -eval(n : NumberNode*) : double
        -eval(v : VariableNode*) : double
        -eval(b : BinaryOpNode*) : double
        -eval(u : UnaryOpNode*) : double
        -eval(f : UnaryFunctionNode*) : double
    }

    class Utils {
        +trim(s : string) : string
        +readFile(filename : string) : string
        +splitSessions(fileContent : string) : vector<string>
        +formatDouble(value : double) : string
    }

    class SimpleStack~T~ {
        <<template>>
        +push(x : T) : void
        +pop() : T
        +top() : T&
        +empty() : bool
    }

    Lexer --> Token : returns
    Parser --> Lexer : reads tokens
    Parser --> ASTNode : builds
    ASTNode <|-- NumberNode
    ASTNode <|-- VariableNode
    ASTNode <|-- BinaryOpNode
    ASTNode <|-- UnaryOpNode
    ASTNode <|-- UnaryFunctionNode

    BinaryOpNode *-- ASTNode : owns left/right
    UnaryOpNode *-- ASTNode : owns child
    UnaryFunctionNode *-- ASTNode : owns arg

    Evaluator --> ASTNode : evaluates
    Evaluator --> SymbolTable : lookup/set

    Utils ..> Lexer : helpers
    Utils ..> Parser : helpers
