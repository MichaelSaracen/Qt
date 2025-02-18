const sqlite3 = require("sqlite3");

function getAll() {
    return new Promise( (resolve, reject ) => {
        const db = new sqlite3.Database("../db/kino.db");

        db.all(`
        SELECT movie.title as t, genre.name as n, spielzeiten.datum as d, saal.name as sn
        FROM spielzeiten
        JOIN movie ON movie.movieID = spielzeiten.movieID
        JOIN genre ON movie.genreID = genre.genreID
        JOIN saal ON saal.saalID = spielzeiten.saalID
        `, (err, rows) => {
            if ( err ) {
                console.error( err );
                reject( err );
            } else {
                let html = ''
                for ( const row of rows )
                    html += `${row.t}, ${row.n}, ${row.d}, ${row.sn}  <br><br>`
                resolve( html );
            }

            db.close();
        })
    })
}

module.exports = { getAll }