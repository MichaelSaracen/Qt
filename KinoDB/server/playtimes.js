const sqlite3 = require("sqlite3");

function getAllPlaytimes() {
    return new Promise( (resolve, reject ) => {
        const db = new sqlite3.Database("../db/kino.db");

        db.all(`
        SELECT * FROM spielzeiten

`, (err, rows) => {
            if ( err ) {
                console.error( err );
                reject( err );
            } else {
                let html = ''
                for ( const row of rows )
                    html += `${row.spielzeitenID}, ${row.datum}, ${row.saalID}, ${row.movieID}  <br><br>`
                resolve( html );
            }

            db.close();
        })
    })
}

module.exports = { getAllPlaytimes }