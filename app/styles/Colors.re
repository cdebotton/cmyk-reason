let neutral = [
  "hsl(255, 80%, 50%)",
  "hsl(245, 50%, 51%)",
  "hsl(235, 50%, 52%)",
  "hsl(225, 50%, 53%)",
  "hsl(215, 50%, 53%)",
];

let negative = [
  "hsl(0, 80%, 50%)",
  "hsl(10, 70%, 51%)",
  "hsl(20, 70%, 52%)",
  "hsl(30, 70%, 53%)",
  "hsl(40, 70%, 53%)",
];

let positive = [
  "hsl(120, 70%, 50%)",
  "hsl(130, 70%, 50%)",
  "hsl(140, 70%, 50%)",
  "hsl(150, 70%, 50%)",
  "hsl(160, 70%, 50%)",
];

let light = [
  "hsl(0, 0%, 100%)",
  "hsl(0, 0%, 95%)",
  "hsl(0, 0%, 90%)",
  "hsl(0, 0%, 85%)",
  "hsl(0, 0%, 80%)",
];

let dark = [
  "hsl(0, 0%, 0%)",
  "hsl(0, 0%, 5%)",
  "hsl(0, 0%, 10%)",
  "hsl(0, 0%, 15%)",
  "hsl(0, 0%, 20%)",
];

type t =
  | Neutral
  | Negative
  | Positive
  | Dark
  | Light;

let getColorGroup = color =>
  switch (color) {
  | Neutral => neutral
  | Negative => negative
  | Positive => positive
  | Dark => dark
  | Light => light
  };

let getColor = (color, at) => color |> getColorGroup |. List.nth(at);