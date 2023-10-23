--Filtered all the "crimes" that happened on 28/7 and Humphrey street, found 2 results, the one is the correct (new clue: hour 10:15)

SELECT description FROM crime_scene_reports
WHERE month == 7 AND day == 28 and street == "Humphrey Street"


--Filtered the interviews to the day that the crime happened Ruth, Eugene and Raymond gave clues.

SELECT transcript , name FROM interviews
WHERE month == 7 AND day == 28

--By checking the bakery security logs as per the interviews, I found the people person who left the bakery with the car at the designated day and somewhere after around 10:15

SELECT people.name , bakery_security_logs.license_plate
FROM bakery_security_logs
INNER JOIN people on bakery_security_logs.license_plate == people.license_plate
WHERE month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND activity = 'exit';

--Vanessa	5P2BI95
--Bruce	94KL13X
--Barry	6P58WS2
--Luca	4328GD8
--Sofia	G412CB7
--Iman	L93JTIZ
--Diana	322W7JE
--Kelsey	0NTHK55
--Taylor	1106N58


--By combining all the clues, we have a narrowed down list of 3 people

--name	plate	phone		account_num
--Taylor	1106N58	(286) 555-6063	(310) 555-8568	76054385
--Bruce	94KL13X	(367) 555-5533	(022) 555-4052	49610011
--Bruce	94KL13X	(367) 555-5533	(344) 555-9601	49610011
--Bruce	94KL13X	(367) 555-5533	(704) 555-5790	49610011
--Barry	6P58WS2	(301) 555-4174	(711) 555-3007	56171033


SELECT DISTINCT people.name , bakery_security_logs.license_plate , people.phone_number, phone_calls.receiver , bank_accounts.account_number
FROM bakery_security_logs
INNER JOIN people on bakery_security_logs.license_plate == people.license_plate
INNER JOIN bank_accounts ON bank_accounts.person_id == people.id
INNER JOIN atm_transactions ON atm_transactions.account_number == bank_accounts.account_number
INNER JOIN phone_calls ON phone_calls.caller == people.phone_number
WHERE atm_transactions.month == 7 AND atm_transactions.day == 28
AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >= 15 AND activity = 'exit'
AND phone_calls.duration<60 AND phone_calls.month = 7 AND phone_calls.day = 28


--By adding all the flights that will take place tomorrow we narrow down the list to Three people. (Made a mistake and filtered all calls >60 instead of <60)

--Name:	Made a call to:		Airport	Going to:
---------------------------------------------------
--Bruce	(375) 555-8161		8	4
--Diana	(725) 555-3243		8	6
--Taylor	(676) 555-6554		8	4

SELECT people.name , phone_calls.receiver,flights.origin_airport_id, flights.destination_airport_id
FROM bakery_security_logs
INNER JOIN people on bakery_security_logs.license_plate == people.license_plate
INNER JOIN bank_accounts ON bank_accounts.person_id == people.id
INNER JOIN atm_transactions ON atm_transactions.account_number == bank_accounts.account_number
INNER JOIN phone_calls ON phone_calls.caller == people.phone_number
INNER JOIN passengers ON passengers.passport_number == people.passport_number
INNER JOIN flights ON flights.id == passengers.flight_id
WHERE atm_transactions.month == 7 AND atm_transactions.day == 28
AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >= 15 AND activity = 'exit'
AND phone_calls.duration<60 AND phone_calls.month = 7 AND phone_calls.day = 28
AND flights.month==7 AND flights.day == 29


-- To clear things up about accomplices, destinations and airports I tidied things up by modyfying the query a little, I also added a flight hour clause in order
to check which flight is the earliest, which seems to be at 8:20, Bruce and Taylor is in them.


SELECT
  people.name AS Suspect, receiver_name.name AS Accomplice, origin_airports.city AS Airport, destination_airports.city AS Destination, flights.hour, flights.minute
FROM
  bakery_security_logs
  INNER JOIN people
	ON bakery_security_logs.license_plate = people.license_plate
  INNER JOIN bank_accounts
	ON bank_accounts.person_id = people.id
  INNER JOIN atm_transactions
	ON atm_transactions.account_number = bank_accounts.account_number
  INNER JOIN phone_calls
	ON phone_calls.caller = people.phone_number
  INNER JOIN passengers
	ON passengers.passport_number = people.passport_number
  INNER JOIN flights
	ON flights.id = passengers.flight_id
  INNER JOIN airports AS origin_airports
	ON origin_airports.id = flights.origin_airport_id
  INNER JOIN airports AS destination_airports
	ON destination_airports.id = flights.destination_airport_id
  INNER JOIN people AS receiver_name
	ON receiver_name.phone_number = phone_calls.receiver
WHERE
  atm_transactions.month = 7 AND atm_transactions.day = 28
  AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28 AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >= 15
  AND activity = 'exit'
  AND phone_calls.duration < 60 AND phone_calls.month = 7 AND phone_calls.day = 28
  AND flights.month = 7 AND flights.day = 29;

--I Ignored a hint which seemed to be essential, it seems that the thief left the car at a 10 min time window which makes taylor (who apparently left later)
--not suitable to be the thief. The thief also took THE EARLIEST tommorow's plane which also makes Diane which left at 16:00 not suitable to be the thief. It
--all narrows down to Bruce (Suspect), which made a small phone call to Robin (accomplice) and left to New york City (Destination) and here is the query that
--proves all of that.

--Case dismissed. I wanna thank my aid, SQL for helping me out narrowing down the clues.

SELECT
  people.name AS Suspect, receiver_name.name AS Accomplice, origin_airports.city AS Airport, destination_airports.city AS Destination, flights.hour AS Depart_Hour, flights.minute AS Depart_Minutes
FROM
  bakery_security_logs
  INNER JOIN people
	ON bakery_security_logs.license_plate = people.license_plate
  INNER JOIN bank_accounts
	ON bank_accounts.person_id = people.id
  INNER JOIN atm_transactions
	ON atm_transactions.account_number = bank_accounts.account_number
  INNER JOIN phone_calls
	ON phone_calls.caller = people.phone_number
  INNER JOIN passengers
	ON passengers.passport_number = people.passport_number
  INNER JOIN flights
	ON flights.id = passengers.flight_id
  INNER JOIN airports AS origin_airports
	ON origin_airports.id = flights.origin_airport_id
  INNER JOIN airports AS destination_airports
	ON destination_airports.id = flights.destination_airport_id
  INNER JOIN people AS receiver_name
	ON receiver_name.phone_number = phone_calls.receiver
WHERE
  atm_transactions.month = 7
  AND atm_transactions.day = 28
  AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28 AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >= 15 AND bakery_security_logs.minute <= 25 AND activity = 'exit'
  AND phone_calls.duration < 60 AND phone_calls.month = 7 AND phone_calls.day = 28
  AND (flights.hour, flights.minute) = (
    SELECT flights.hour, flights.minute
    FROM flights
    WHERE flights.month = 7 AND flights.day = 29
    ORDER BY flights.hour, flights.minute
  )
ORDER BY flights.hour, flights.minute;