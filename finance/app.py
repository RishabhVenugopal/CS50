import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

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
    sum = 0
    rows = db.execute(
        "SELECT symbol, SUM(shares) FROM transactions WHERE user_id = ? GROUP BY symbol",
        session["user_id"],
    )
    for row in rows:
        row["price"] = round(lookup(row["symbol"])["price"], 2)
        sum = sum + row["price"] * row["SUM(shares)"]
    balance = round(
        db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
            "cash"
        ],
        2,
    )
    return render_template("index.html", rows=rows, balance=balance, sum=sum, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        user_id = session["user_id"]
        balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        if not symbol:
            return apology("Must enter symbol")
        if not shares:
            return apology("Must enter Number of shares")
        if not shares.isdigit():
            return apology("Please enter a positive integer")
        quote = lookup(symbol)
        if not quote:
            return apology("Stock doesn't exist")
        if balance < quote["price"] * float(shares):
            return apology("Insufficient Funds")
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            str(round(balance - (quote["price"] * float(shares)))),
            user_id,
        )
        date = datetime.now().date().strftime("%d-%m-%y")
        time = datetime.now().time().strftime("%H:%M:%S")
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date, time) VALUES (?, ?, ?, ?, ?, ?)",
            user_id,
            symbol,
            shares,
            quote["price"],
            date,
            time,
        )
        return redirect("/")
    else:
        buy_button = request.args.get("buy")
        if buy_button:
            return render_template("buy.html", index=True, symbol=buy_button)
        else:
            return render_template("buy.html", index=False)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        "SELECT symbol, shares, price, date, time FROM transactions WHERE user_id = ?",
        session["user_id"],
    )
    return render_template("history.html", rows=rows, abs=abs, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        if quoted:
            return render_template(
                "quoted.html", symbol=quoted["symbol"], price=usd(quoted["price"])
            )
        else:
            return apology("Stock doesn't exist")
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("Must provide username")
        elif not password:
            return apology("Must provide password")
        elif not confirmation:
            return apology("Please confirm password")
        elif password != confirmation:
            return apology("Passwords do not match")
        elif (
            len(db.execute("SELECT username FROM users WHERE username = ?", username))
            == 1
        ):
            return apology("Username already exists")
        else:
            db.execute(
                "INSERT INTO users(username, hash) VALUES(?, ?)",
                username,
                generate_password_hash(password),
            )
            return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[
            0
        ]["cash"]
        if not symbol:
            return apology("Must enter symbol")
        if not shares:
            return apology("Must enter Number of shares")
        if not shares.isdigit():
            return apology("Please enter NUMBER of shares")
        shares = int(shares)
        if shares < 0:
            return apology("Please enter positive number of shares")
        quote = lookup(symbol)
        if not quote:
            return apology("Stock doesn't exist")
        owned_shares = db.execute(
            "SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
            session["user_id"],
            symbol,
        )
        if not owned_shares or owned_shares[0]["SUM(shares)"] == 0:
            return apology("You don't own this stock")
        owned_shares = owned_shares[0]["SUM(shares)"]
        if owned_shares < shares:
            return apology("You don't own enough of this stock")
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            str(round(balance + (quote["price"] * float(shares)))),
            session["user_id"],
        )
        date = datetime.now().date().strftime("%d-%m-%y")
        time = datetime.now().time().strftime("%H:%M:%S")
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date, time) VALUES(?, ?, ?, ?, ?, ?)",
            session["user_id"],
            symbol,
            -shares,
            quote["price"],
            date,
            time,
        )
        return redirect("/")
    else:
        rows = db.execute(
            "SELECT symbol, SUM(shares) FROM transactions WHERE user_id = ? GROUP BY symbol",
            session["user_id"],
        )
        sell_button = request.args.get("sell")
        if sell_button:
            return render_template(
                "sell.html", rows=rows, index=True, symbol=sell_button
            )
        else:
            return render_template("sell.html", rows=rows, index=False)
