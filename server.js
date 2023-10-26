import express from "express";
const app = express();
const PORT = 3000;

let relayState = "OFF";

app.use(express.static("public"));

app.get("/update", (req, res) => {
  const { temp, hum, moist, relay } = req.query;
  console.log(`Temp: ${temp} Hum: ${hum} Moist: ${moist} Relay: ${relay}`);
  relayState = relay === "HIGH" ? "ON" : "OFF";
  res.sendStatus(200).send(relayState);
});

app.get("/override", (req, res) => {
  relayState = relayState === "ON" ? "OFF" : "ON";
  res.sendStatus(200).send(relayState);
});

app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
