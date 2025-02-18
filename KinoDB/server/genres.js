const sqlite3 = require("sqlite3");

function getAllGenres() {
    return new Promise( (resolve, reject ) => {
        const db = new sqlite3.Database("../db/kino.db");

        db.all("SELECT * FROM genre", (err, rows) => {
            if ( err ) {
                console.error( err );
                reject( err );
            } else {
                let html = ''
                for ( const row of rows )
                    html += `${row.genreID} ${row.name}<br><br>`
                resolve( html );
            }

            db.close();
        })
    })
}

module.exports = { getAllGenres }