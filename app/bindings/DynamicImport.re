type importable('a) = {
  .
  "__esModule": bool,
  "$$default": bool,
  [@bs.meth] "propertyIsEnumerable": string => bool,
};

[@bs.scope "Object"] [@bs.val]
external depack: importable('a) => 'a = "values";

[@bs.val] external import: string => Js.Promise.t(importable('a)) = "";

let (||>) = (f, g, x) => g(f(x));
let (<$>) = (a, b) => Js.Promise.(then_(b ||> resolve, a));
let (<$!>) = (a, b) => Js.Promise.(catch(b ||> resolve, a));
let (>>=) = (a, b) => Js.Promise.then_(b, a);
let (>>=!) = (a, b) => Js.Promise.catch(b, a);
let (=<<) = (a, b) => Js.Promise.then_(a, b);
let (!=<<) = (a, b) => Js.Promise.catch(a, b);

let depack = x => {
  if (x##propertyIsEnumerable("$$default")
      && x##propertyIsEnumerable("__esModule")
      &&
      x##__esModule) {
    [%raw {| delete x.__esModule |}] |> ignore;
  };
  [%raw {| delete x.$$default |}] |> ignore;
  depack(x);
};

let resolve = fetch => fetch <$> depack;