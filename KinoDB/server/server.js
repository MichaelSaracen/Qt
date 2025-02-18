const { getAllMovies } = require('./movies.js');
const { getAllGenres } = require("./genres.js");
const { getAllSaales } = require("./saal.js");
const { getAllPlaytimes } = require("./playtimes.js");
const { getAll } = require("./cinemaview.js");
const http = require("http");

const server = http.createServer((req, res) => {
    if (req.url === "/") {
        Promise.all([getAllMovies(), getAllGenres(), getAllSaales(), getAllPlaytimes(), getAll()])
            .then(result => {
                const [moviesHtml, genresHtml, saalHtml, playtimeHtml, allHtml] = result;

                const html = `
                <!DOCTYPE html>
                <html lang="de">
                <head>
                    <meta charset="UTF-8">
                    <meta name="viewport" content="width=device-width, initial-scale=1.0">
                    <title>Kino Übersicht</title>
                    <style>
                        body {
                            font-family: Arial, sans-serif;
                            background-color: #f4f4f4;
                            margin: 0;
                            padding: 20px;
                        }
                        .container {
                            max-width: 800px;
                            margin: auto;
                            background: white;
                            padding: 20px;
                            border-radius: 10px;
                            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                        }
                        .section {
                            margin-bottom: 20px;
                            padding: 15px;
                            background: #fff;
                            border-radius: 8px;
                            box-shadow: 0 0 5px rgba(0, 0, 0, 0.1);
                        }
                        h2 {
                            border-bottom: 2px solid #ddd;
                            padding-bottom: 5px;
                            color: #333;
                        }
                    </style>
                </head>
                <body>
                    <div class="container">
                        <div id="genres-container" class="section">
                            <h2>Genres</h2>
                            ${genresHtml}
                        </div>
                        <div id="movies-container" class="section">
                            <h2>Filme</h2>
                            ${moviesHtml}
                        </div>
                        <div id="saal-container" class="section">
                            <h2>Säle</h2>
                            ${saalHtml}
                        </div>
                        <div id="playtime-container" class="section">
                            <h2>Spielzeiten</h2>
                            ${playtimeHtml}
                        </div>
                        <div id="all-container" class="section">
                            <h2>Gesamtübersicht</h2>
                            ${allHtml}
                        </div>
                    </div>

                    <script>
                        async function fetchMovies() {
                            try {
                                let response = await fetch("/");
                                let text = await response.text();
                                let parser = new DOMParser();
                                let newDocument = parser.parseFromString(text, "text/html");

                                document.getElementById("movies-container").innerHTML = newDocument.getElementById("movies-container").innerHTML;
                                document.getElementById("genres-container").innerHTML = newDocument.getElementById("genres-container").innerHTML;
                                document.getElementById("saal-container").innerHTML = newDocument.getElementById("saal-container").innerHTML;
                                document.getElementById("playtime-container").innerHTML = newDocument.getElementById("playtime-container").innerHTML;
                                document.getElementById("all-container").innerHTML = newDocument.getElementById("all-container").innerHTML;
                            } catch (error) {
                                console.error("Fehler beim Abrufen der Daten:", error);
                            }
                        }

                        setInterval(fetchMovies, 500); 
                    </script>
                </body>
                </html>
                `;

                res.writeHead(200, { "Content-type": "text/html" });
                res.end(html);
            })
            .catch(err => {
                res.writeHead(500, { "Content-type": "text/html" });
                res.end("Fehler beim Laden der Daten: " + err.message);
            });
    } else {
        res.writeHead(404, { "Content-type": "text/html" });
        res.end("Fatal error");
    }
});

server.listen(8080, "127.0.0.1", () => {
    console.log("Verbunden mit localhost");
});
