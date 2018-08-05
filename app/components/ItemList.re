module Styles = {
  let list =
    Css.(style([listStyleType(none), margin(0 |. px), padding(0 |. px)]));
};

let component = ReasonReact.statelessComponent("List");

let make = (~items, ~renderItem, ~getKey, _children) => {
  ...component,
  render: _self =>
    <ul className=Styles.list>
      (
        items
        |. Belt.Array.map(item =>
             <li key=(getKey(item))> (renderItem(item)) </li>
           )
        |> ReasonReact.array
      )
    </ul>,
};