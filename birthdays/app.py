import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)
app.secret_key = '&^$%&&8678^*%^&*8*%7^&*(&)'
# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        try:
            name = str(request.form.get("name"))
            month = int(request.form.get("month"))
            day = int(request.form.get("day"))
        except:
            flash("Variable type mismatch")
            return redirect("/")
        else:
            # Remember registrant
            if month>12 or month<1:
                flash("Invalid number insertion on months")
                return redirect("/")
            elif day<1 or day>31:
                flash("Invalid number insertion on days")
                return redirect("/")
            else:

                db.execute("INSERT INTO birthdays (name, month , day) VALUES( ? , ? , ?)" , name, month,day)

                # Confirm registration

                return redirect("/")

    else:
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthday=birthdays)

"""
Implemented a delete button that would be placed in each row, when you press it, it will delete the row it needs. It's not required thought so I commented it out.
@app.route("/delete/<int:birthday_id>", methods=["POST"])
def delete(birthday_id):
    db.execute("DELETE FROM birthdays WHERE id = ?", birthday_id)
    return redirect("/")
"""


