const model = require('../model/detect')

var myList;

module.exports = {
    //Get the home page
    getHomePage: (req, res) => {
        res.sendFile(process.cwd() + '/project/api/view/index.html')


    },

    // POST search
    postDetect: (req, res) => {

        if (!req.files) {
            res.write('Please choose normal and anomaly files')

        } else if (!req.files.normal_file) {
            res.write('Please choose normal file')

        } else if (!req.files.anomaly_file) {
            res.write('Please choose anomaly file')

        } else {


            let algoType = req.body.algos
            //res.write('The anomalys in ' + algoType + ' algo are:\n')
            let normalFile = req.files.normal_file
            let anomalyFile = req.files.anomaly_file

            var prePath = 'files/';

            var normalPath = prePath + 'normalFile.txt';
            var contentNormal = normalFile.data.toString();
            saveFile(normalPath, contentNormal);

            var anomalyPath = prePath + 'anomalyFile.txt';
            var contentAnomaly = anomalyFile.data.toString();
            saveFile(anomalyPath, contentAnomaly);

            myList = model.detectAnomaly(normalPath, anomalyPath, algoType);
            myList = myList.anomalies;

            if (typeof myList[0]['reason'] === 'undefined') {
                res.write('There is no Anomalys');
            } else {
                var selector = '';
                selector += '<style>table {font-family: arial, sans-serif;border-collapse: collapse;width: 100%;} td, th {border: 1px solid #dddddd; text-align: left;padding: 8px;} tr:nth-child(even) {background-color: #dddddd;}</style>';
                selector += '<table>';

                selector += buildHtmlTable();

                selector += '</table >';
                res.write('<script>var x = document.getElementById("loader");x.style.display = "none";</script>');
                res.write(selector)
            }

        }
        res.end();

    }

}
function isEmpty(obj) {
    for (var prop in obj) {
        if (obj.hasOwnProperty(prop))
            return false;
    }

    return true;
}
function sleepFor(sleepDuration) {
    var now = new Date().getTime();
    while (new Date().getTime() < now + sleepDuration) { /* do nothing */ }
}

function saveFile(path, content) {
    var fs = require('fs');
    fs.writeFileSync(path, content, function (err) {
        if (err) {
            console.log('Error saved File ');
            throw err;
        }

        console.log('Success saved File');
    });
}


// Builds the HTML Table out of myList.
function buildHtmlTable() {
    var mySelector = '';
    mySelector += addAllColumnHeaders();

    for (var i = 0; i < myList.length; i++) {

        var rowHash = myList[i];
        mySelector += '<tr>';
        for (var colIndex = 0; colIndex < 3; colIndex++) {
            var cellValue


            if (cellValue == null) {
                cellValue = ""
            }

            //Get Anomalys key to the first coulmn
            if (colIndex == 0) {
                for (var key in rowHash) {
                    cellValue = key
                    //cellValue = cellValue.replace('  ', '<br>')
                    break
                }

                //Get the Start and End Time
            } else if (colIndex == 1) {
                var myKey
                for (var key in rowHash) {
                    myKey = key
                    break
                }
                cellValue = myList[i][myKey]

                //Get the reason
            } else if (colIndex == 2) {
                cellValue = myList[i]['reason']
            } else {
                break;
            }

            //row$.append($('<td/>').html(cellValue));
            mySelector += '<td>' + cellValue + '</td>';
        }
        //$(selector).append(row$);
        mySelector += '</tr>';
    }
    return mySelector;
}

// Adds a header row to the table and returns the set of columns.
// Need to do union of keys from all records as some records may not contain
// all records.
function addAllColumnHeaders() {
    var mySelector = '';
    mySelector += '<tr>'

    mySelector += '<th>Anomalys</th>'
    mySelector += '<th>Start, End</th>'
    mySelector += '<th>Reason</th>'

    mySelector += '</tr>'

    return mySelector;
}