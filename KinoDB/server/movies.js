const sqlite3 = require("sqlite3");

function getAllMovies() {
    return new Promise( (resolve, reject) => {
        const db = new sqlite3.Database("../db/kino.db");

        db.all( `SELECT * FROM movie`, (err, rows) => {
            if ( err ) {
                console.error( err );
                reject( err );
            }  else {
                let html = '';
                for ( const row of rows ) {
                    html += `${row.movieID}, ${row.title}, ${row.genreID}, ${row.fsk}, ${row.duration} <br><br>`
                }
                resolve(html)
            }
            db.close()
        })
    })
}

module.exports = { getAllMovies }