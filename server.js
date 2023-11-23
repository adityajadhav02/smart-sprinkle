import express from "express";
const app = express();
const PORT = 3000;

let relayState = "OFF";

let temperature = -1;
let moisture = -1;
let humidity = -1;
// app.use(express.static("public"));

app.use(
  express.static("public", {
    setHeaders: (res, path, stat) => {
      if (path.endsWith(".js")) {
        res.set("Content-Type", "application/javascript");
      }
    },
  })
);

app.get("/update", (req, res) => {
  const { temp, hum, moist, relay } = req.query;
  relayState = relay === "HIGH" ? "ON" : "OFF";
  temperature = temp;
  humidity = hum;
  moisture = moist;

  res.status(200);
});

app.get("/sendData", (req, res) => {
  const sensorData = {
    temperature: temperature,
    relay: relayState,
    humidity: humidity,
    moisture: moisture,
  };

  res.status(200).json(sensorData);
});

app.get("/override", (req, res) => {
  relayState = relayState === "ON" ? "OFF" : "ON";
  res.status(200).send(relayState);
});

app.get("/getOverrideStatus", (req, res) => {
  res.send(relayState);
});

const server = app.listen(PORT, "192.168.221.108", () => {
  const host = server.address().address;
  const port = server.address().port;
  console.log(`Server is running on http://${host}:${port}`);
});