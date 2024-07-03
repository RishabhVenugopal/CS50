import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")
months = ["NULL", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    rows = db.execute("SELECT * FROM birthdays")
    if request.method == "POST":
        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        if name and day and month:
            if day.isdigit() and int(day) >= 1 and int(day) <= 31 and month.isdigit() and int(month) >= 1 and int(month) <= 12:
                db.execute("INSERT INTO birthdays(name, month, day) VALUES(?, ?, ?)", name, month, day)
        for i in range(0, len(rows)):
            if "button" + str(rows[i]["id"]) in request.form:
                db.execute("DELETE FROM birthdays WHERE id=?", rows[i]["id"])
            if "edit" + str(rows[i]["id"]) in request.form:
                editname = request.form.get("name" + str(rows[i]["id"]))
                editday = request.form.get("day" + str(rows[i]["id"]))
                editmonth = request.form.get("month" + str(rows[i]["id"]))
                if editname and editday and editmonth:
                    if editday.isdigit() and editmonth.isdigit() and int(editday) > 0 and int(editmonth) > 0 and int(editday) <= 31 and int(editmonth) <= 12:
                        db.execute("UPDATE birthdays SET name = ?, day = ?, month = ? WHERE id = ?", editname, editday, editmonth, str(rows[i]["id"]))
        return redirect("/")


    else:
        # TODO: Display the entries in the database on index.html
        return render_template("index.html", rows=rows, months=months)

