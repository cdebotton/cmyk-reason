let arrayToList = a => {
  let rec tolist = (i, res) =>
    if (i < 0) {
      res;
    } else {
      tolist(i - 1, [Array.unsafe_get(a, i), ...res]);
    };
  tolist(Array.length(a) - 1, []);
};

let joinList = (~sep=", ", list) => {
  let rec iter = (search, first, result) =>
    switch (search, first, result) {
    | ([], _, _) => result
    | ([item, ...rest], true, _result) => iter(rest, false, item)
    | ([item, ...rest], false, result) =>
      iter(rest, false, {j|$result$sep$item|j})
    };
  iter(list, true, "");
};

let unwrapOptionalList = list => {
  let rec iter = (list, result) =>
    switch (list) {
    | [Some(item), ...rest] => iter(rest, [item, ...result])
    | [None, ...rest] => iter(rest, result)
    | [] => result
    };

  iter(list, []);
};
