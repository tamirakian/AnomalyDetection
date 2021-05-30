# AnomalyDetection
AnomalyDetection Contributes: Tamir Akian , Gabriel Cembal , Adam Peretz, Orad Telner.

A description on the main features:
When entering the server's IP the user will have two algorithims to choose from:
1) Regression - This algorithim detects anomalies in a flight based on a linear regression formula. 
2) Hybrid - This algorithim detects anomalies in a flight based on a the min circle that contains all given points.
The user will upload two files, one of a normal flight and the other one of a flight with anomalies.
The anomalies will be desplayed in an easy to read data table. 
First time running:
*) Download Node.js
*) Download Python 2.7
1) clone from git repository.
2) open CMD from the repository directory.
3) execute via CMD the following commands:
	a)npm install -S node-addon-api
	b)npm install -S -g node-addon-api
	c)npm install express
	d)npm install -g express
	e)npm install nodemon
	f)npm install -g nodemon
	g)npm i express-fileupload
	h)npm -g i express-fileupload
	i)npm install node-gyp
	j)npm install -g node-gyp

	k)node-gyp configure
	l)node-gyp build

	m)npm start
4) open a web browser and type in: http://localhost:8080/

The link to the UML: https://ibb.co/tDMtz8h

The link to the explenation video: https://drive.google.com/file/d/1uSQHJNocU3cxB_KobmltdSNDI14UbCPU/view
