class Calculator {
  constructor() {
    this.stack = [];
  }

  isOperator(c) {
    return ["+", "-", "*", "/", "%", "^"].includes(c);
  }

  precedence(op) {
    if (op === "+" || op === "-") return 1;
    if (op === "*" || op === "/" || op === "%") return 2;
    if (op === "^") return 3; // highest precedence
    return 0;
  }

  infixToPostfix(infix) {
    if (!this.isValidInfix(infix)) {
      return "Invalid expression";
    }
    let result = "";
    let operand = "";
    for (let i = 0; i < infix.length; i++) {
      let c = infix[i];
      if (/\s/.test(c)) {
        if (operand) {
          result += operand + " ";
          operand = "";
        }
        continue;
      }

      if (/\d/.test(c)) {
        operand += c;
      } else {
        if (operand) {
          result += operand + " ";
          operand = "";
        }
        if (c === "(") {
          this.stack.push(c);
        } else if (c === ")") {
          while (
            this.stack.length &&
            this.stack[this.stack.length - 1] !== "("
          ) {
            result += this.stack.pop() + " ";
          }
          this.stack.pop();
        } else {
          while (
            this.stack.length &&
            this.precedence(this.stack[this.stack.length - 1]) >=
              this.precedence(c)
          ) {
            result += this.stack.pop() + " ";
          }
          this.stack.push(c);
        }
      }
    }

    if (operand) result += operand + " ";
    while (this.stack.length) result += this.stack.pop() + " ";
    return result.trim();
  }

  infixToPrefix(infix) {
    if (!this.isValidInfix(infix)) return "Invalid expression";

    let reversed = this.reverseString(infix);
    reversed = reversed
      .replace(/\(/g, "temp")
      .replace(/\)/g, "(")
      .replace(/temp/g, ")");
    let postfix = this.infixToPostfix(reversed);
    return this.reverseString(postfix);
  }

  evaluateInfix(infix) {
    let postfix = this.infixToPostfix(infix);
    return this.evaluatePostfix(postfix);
  }

  postfixToInfix(postfix) {
    this.stack = [];
    let tokens = postfix.split(" ");
    for (let token of tokens) {
      if (/\d/.test(token)) {
        this.stack.push(token);
      } else {
        let b = this.stack.pop();
        let a = this.stack.pop();
        this.stack.push(`(${a} ${token} ${b})`);
      }
    }
    return this.stack.pop();
  }

  postfixToPrefix(postfix) {
    this.stack = [];
    let tokens = postfix.split(" ");
    for (let token of tokens) {
      if (/\d/.test(token)) {
        this.stack.push(token);
      } else {
        let b = this.stack.pop();
        let a = this.stack.pop();
        this.stack.push(`${token} ${a} ${b}`);
      }
    }
    return this.stack.pop();
  }

  prefixToInfix(prefix) {
    this.stack = [];
    let tokens = prefix.split(" ").reverse();
    for (let token of tokens) {
      if (/\d/.test(token)) {
        this.stack.push(token);
      } else {
        let a = this.stack.pop();
        let b = this.stack.pop();
        this.stack.push(`(${a} ${token} ${b})`);
      }
    }
    return this.stack.pop();
  }

  prefixToPostfix(prefix) {
    this.stack = [];
    let tokens = prefix.split(" ").reverse();
    for (let token of tokens) {
      if (/\d/.test(token)) {
        this.stack.push(token);
      } else {
        let a = this.stack.pop();
        let b = this.stack.pop();
        this.stack.push(`${a} ${b} ${token}`);
      }
    }
    return this.stack.pop();
  }

  evaluatePostfix(postfix) {
    this.stack = [];
    let tokens = postfix.split(" ");
    for (let token of tokens) {
      if (/\d/.test(token)) {
        this.stack.push(parseInt(token));
      } else {
        let b = this.stack.pop();
        let a = this.stack.pop();
        switch (token) {
          case "+":
            this.stack.push(a + b);
            break;
          case "-":
            this.stack.push(a - b);
            break;
          case "*":
            this.stack.push(a * b);
            break;
          case "/":
            this.stack.push(a / b);
            break;
          case "%":
            this.stack.push(a % b);
            break;
          case "^":
            this.stack.push(Math.pow(a, b));
            break;
        }
      }
    }
    return this.stack.pop();
  }

  evaluatePrefix(prefix) {
    return this.evaluatePostfix(this.prefixToPostfix(prefix));
  }

  reverseString(str) {
    return str.split("").reverse().join("");
  }

  isValidInfix(infix) {
    let balance = 0;
    for (let i = 0; i < infix.length; i++) {
      if (infix[i] === "(") balance++;
      if (infix[i] === ")") balance--;
      if (balance < 0) return false;
    }
    return balance === 0;
  }
}

// Functions to handle button clicks
const calculator = new Calculator();

function convertToPostfix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.infixToPostfix(expression);
  document.getElementById("output").innerText = "Postfix: " + result;
}

function convertToPrefix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.infixToPrefix(expression);
  document.getElementById("output").innerText = "Prefix: " + result;
}

function evaluateExpression() {
  const expression = document.getElementById("expression").value;
  const result = calculator.evaluateInfix(expression);
  document.getElementById("output").innerText = "Result: " + result;
}

function convertPostfixToInfix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.postfixToInfix(expression);
  document.getElementById("output").innerText = "Infix: " + result;
}

function convertPostfixToPrefix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.postfixToPrefix(expression);
  document.getElementById("output").innerText = "Prefix: " + result;
}

function convertPrefixToInfix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.prefixToInfix(expression);
  document.getElementById("output").innerText = "Infix: " + result;
}

function convertPrefixToPostfix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.prefixToPostfix(expression);
  document.getElementById("output").innerText = "Postfix: " + result;
}

function evaluatePrefix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.evaluatePrefix(expression);
  document.getElementById("output").innerText = "Result: " + result;
}

function evaluatePostfix() {
  const expression = document.getElementById("expression").value;
  const result = calculator.evaluatePostfix(expression);
  document.getElementById("output").innerText = "Result: " + result;
}
