#include <string>

class Token {
 public:
  enum class Kind {
    Real,
    Identifier,
    Keyword,
    Operator,
    Separator,
    Unexpected
    ,End
  };

  Token(Kind kind) noexcept : type{kind} {}

  Token(Kind kind, const char* beg, std::size_t len) noexcept
      : type{kind}, lex(beg, len) {}

  Token(Kind kind, const char* beg, const char* end) noexcept
      : type{kind}, lex(beg, std::distance(beg, end)) {}

  Kind kind() const noexcept { return type; }

  void kind(Kind kind) noexcept { type = kind; }

  bool is_not(Kind kind) const noexcept { return type != kind; }

  bool is(Kind kind) const noexcept { return type == kind; }

  bool is_one_of(Kind k1, Kind k2) const noexcept { return is(k1) || is(k2); }

  template <typename... Ts>
  bool is_one_of(Kind k1, Kind k2, Ts... ks) const noexcept {
    return is(k1) || is_one_of(k2, ks...);
  }

  std::string_view lexeme() const noexcept { return lex; }

  void lexeme(std::string_view lexeme) noexcept {
    lex = std::move(lexeme);
  }

 private:
  Kind             type{};
  std::string_view lex{};
};

class Lexer {
 public:
  Lexer(const char* beg) noexcept : m_beg{beg} {}

  Token next() noexcept;

 private:
  Token identifier() noexcept;
  Token number() noexcept;
  Token cond(Token::Kind) noexcept;

  char peek() const noexcept { return *m_beg; }
  char get() noexcept { return *m_beg++; }

  const char* m_beg = nullptr;
};

bool space(char c) noexcept {
  switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      return true;
    default:
      return false;
  }
}

bool dig(char c) noexcept {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
  }
}

bool is_identifier_char(char c) noexcept {
  switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '_':
      return true;
    default:
      return false;
  }
}

Token Lexer::cond(Token::Kind kind) noexcept { return Token(kind, m_beg++, 1); }

Token Lexer::next() noexcept {
  while (space(peek())) get();

  switch (peek()) {
    case '\0':
      return Token(Token::Kind::End, m_beg, 1);
    default:
      return cond(Token::Kind::Unexpected);
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return identifier();
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return number();
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
      return cond(Token::Kind::Separator);
    case '<':
    case '>':
    case '=':
    case '+':
    case '-':
    case '*':
      return cond(Token::Kind::Operator);
    case '/':
    case '#':
    case ',':
    case ':':
    case '.':
    case ';':
    case '\'':
    case '"':
    case '|':
      return cond(Token::Kind::Separator);

  }
}

Token Lexer::identifier() noexcept {
  const char* start = m_beg;
  get();
  while (is_identifier_char(peek())) get();
  return Token(Token::Kind::Identifier, start, m_beg);
}

Token Lexer::number() noexcept {
  const char* start = m_beg;
  get();
  while (dig(peek())||'.' == peek()) get();
  return Token(Token::Kind::Real, start, m_beg);
}

#include <iomanip>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Token::Kind& kind) {
  static const char* const names[]{
      "Real", "Identifier", 
      "Keyword", "Operator", 
      "Separator", "Unexpected" ,
      "End"
  };
  return os << names[static_cast<int>(kind)];
}
