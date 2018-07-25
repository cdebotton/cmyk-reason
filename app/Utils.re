let arrayToList = a => {
  let rec tolist = (i, res) =>
    if (i < 0) {
      res;
    } else {
      tolist(i - 1, [Array.unsafe_get(a, i), ...res]);
    };
  tolist(Array.length(a) - 1, []);
};