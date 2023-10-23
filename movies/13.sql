SELECT DISTINCT p2.name
FROM people AS p
INNER JOIN stars AS s1 ON s1.person_id = p.id
INNER JOIN movies AS m ON m.id = s1.movie_id
INNER JOIN stars AS s2 ON s2.movie_id = m.id
INNER JOIN people AS p2 ON p2.id = s2.person_id
WHERE p.name = "Kevin Bacon" AND p2.name <> "Kevin Bacon";