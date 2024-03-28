const dotenv = require('dotenv');
const fs = require('fs');
const path = require('path');
const express = require('express');

const app = express();

const envPath = path.join(__dirname, '..', '.env');
dotenv.config({ path: envPath });

const url = process.env.SERVER_URL;
const port = process.env.SERVER_PORT;

const uploadDir = path.join(__dirname, 'uploads');
if (!fs.existsSync(uploadDir)) {
    fs.mkdirSync(uploadDir);
}

app.use(express.urlencoded({ extended: true }));

app.post('/upload', (req, res) => {
    const clientIP = req.connection.remoteAddress;

    let data = '';
    req.on('data', chunk => {
        data += chunk;
    }).on('end', () => {
        const fileName = `${clientIP}-${Date.now()}.txt`;
        const filePath = path.join(uploadDir, fileName);

        fs.writeFile(filePath, data, err => {
            if (err) {
                console.error('Error saving file:', err);
                res.status(500).send('Error saving file');
            } else {
                console.log('File saved successfully:', fileName);
                res.status(200).send('File uploaded successfully');
            }
        });
    });
});

app.use((req, res) => {
    res.status(404).send('Not found');
});

app.listen(port, () => {
    console.log(`Server is listening at http://${url}:${port}`);
});