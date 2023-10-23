import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    # retrieve transaction from the "transactions" table
    trans_db = db.execute(
        "SELECT symbol, SUM(shares) AS shares, price FROM transactions WHERE user_id = ? GROUP BY symbol",
        user_id,
    )
    # Check user's cash
    cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    usercash = cash_db[0]["cash"]
    grand_total = 0

    # Calculate the total value for each transaction and update the "total" key in each row
    for row in trans_db:
        shares = row["shares"]
        price = row["price"]
        total = shares * price
        row["total"] = total
        grand_total += total

    grand_total += usercash
    return render_template(
        "index.html", database=trans_db, cash=usercash, grand_total=grand_total
    )


@app.route("/moremoney", methods=["GET", "POST"])
@login_required
def moremoney():
    """user can add money to his balance"""
    if request.method == "GET":
        return render_template("moremoney.html")
    else:
        add_money = request.form.get("cash")

    if not add_money:
        # See if user inputted any money
        return apology("You have to give some cash")
    user_id = session["user_id"]

    user_id = session["user_id"]
    # Check user's cash
    user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    user_cash = user_cash_db[0]["cash"]

    plus_cash = float(user_cash) + float(add_money)

    db.execute("UPDATE users SET cash = ? WHERE id = ?", plus_cash, user_id)
    flash("Cash added!")
    return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except:
            # In case someone tries to edit the front-end part where input asks only for a number
            return apology("WOW SUCH A HACKER")

    if not symbol:
        return apology("Symbol required")
    # Stock symbols are always on  uppercase so anything on the lookup must be on uppercase
    stock = lookup(symbol.upper())
    if shares < 0:
        return apology("Share must be a positive number")
    try:
        transaction_value = shares * stock["price"]
    except:
        return apology("Symbol not found")

    user_id = session["user_id"]
    # Check user's cache
    user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    user_cash = user_cash_db[0]["cash"]

    # In case user does not that the money
    if user_cash < transaction_value:
        return apology("Not enough cash for purchase")
    else:
        minus_cash = user_cash - transaction_value

    # Save date for history
    date = datetime.datetime.now()

    # deduct cash from the user's balance
    db.execute("UPDATE users SET cash = ? WHERE id = ?", minus_cash, user_id)
    # Add this transaction to the transactions table
    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?,?,?,?,?)",
        user_id,
        stock["symbol"],
        shares,
        stock["price"],
        date,
    )
    flash("Stock bought")
    return redirect("/")


@app.route("/stock_price")
def stock_price():
    """Fetch stock price based on symbol"""
    # Extra route for the ajax to use
    symbol = request.args.get("symbol")

    if not symbol:
        return jsonify({"error": "Symbol required"})

    stock = lookup(symbol)
    if stock:
        price_usd = usd(stock["price"])
        return jsonify({"price": price_usd})
    else:
        return jsonify({"error": "Symbol not found"})


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    # Find all transactions that the user did from the transactions table
    trans_db = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", transactions=trans_db)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        # Searching for a symbol input, if it won't find it returns apology
        if not symbol:
            return apology("Symbol required")
        # Symbols are only upercase so whatever I have to lookup must be on uppercase
        stock = lookup(symbol.upper())
        if stock == None:
            return apology("Symbol not found")
        return render_template(
            "quoteindex.html",
            name=stock["name"],
            price=stock["price"],
            symbol=stock["symbol"],
        )


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")

        # in case username is empty and user somehow bypassed the front-end checks
        if not username:
            return apology("Username field cannot be empty.")

        userrow = db.execute("SELECT * FROM users WHERE username = ?;", username)

        if len(userrow) != 0:
            return apology("Username already taken. Please choose another one.")

        password = request.form.get("password")
        repeat_password = request.form.get("confirmation")

        # in case password is empty and user somehow bypassed the front-end checks
        if not password or not repeat_password:
            return apology("Password fields cannot be empty.")

        # in case repeated password is not equal and user somehow bypassed the front-end checks
        if password != repeat_password:
            return apology("Passwords do not match.")
        # Making password a hash
        hash = generate_password_hash(password, "sha256")
        # Initial cash, we are a generous site.
        cash = 200
        reguser = db.execute(
            "INSERT INTO users (username, hash, cash) VALUES (?, ?, ?)",
            username,
            hash,
            cash,
        )
        session["usid"] = reguser
        # Made a page only for succesful registration
        return render_template("success.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        user_id = session["user_id"]
        # Find all the shares the user has on the db
        symbols_user = db.execute(
            "SELECT symbol, SUM(shares) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
            user_id,
        )
        return render_template("sell.html", symbols=symbols_user)
    else:
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except:
            # In case someone tries to edit the front-end part where input asks only for a number
            return apology("WOW SUCH A HACKER")

        # Just for a safe measure
        if not symbol:
            return apology("Symbol required")

        stock = lookup(symbol.upper())
        if not stock:
            return apology("Invalid symbol")
        # in case he tries to sell negative stocks
        if shares < 0:
            return apology("Shares must be a positive number")

        # Finding the overall value
        transaction_value = shares * stock["price"]

        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash_db[0]["cash"]

        # Checking how many stacks the person has.
        user_shares = db.execute(
            "SELECT SUM(shares) AS total_shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
            user_id,
            symbol,
        )
        if not user_shares or user_shares[0]["total_shares"] < shares:
            return apology("Insufficient shares to sell")

        plus_cash = user_cash + transaction_value

        # Added time, adds more useful information in history
        date = datetime.datetime.now()

        db.execute("UPDATE users SET cash = ? WHERE id = ?", plus_cash, user_id)
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?,?,?,?,?)",
            user_id,
            stock["symbol"],
            -1 * shares,
            stock["price"],
            date,
        )
        flash("Stock Sold")
        return redirect("/")
