module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      flexDirection(row),
      flexWrap(nowrap),
      alignItems(center),
      justifyContent(flexStart),
    ]);

  let title = style([]);

  let message = style([]);
};

let component = ReasonReact.statelessComponent("ApolloError");

let make = (~error: ReasonApolloTypes.apolloError, _children) => {
  ...component,
  render: _self =>
    <div className=Styles.container>
      <Heading className=Styles.title level=2>
        {"Whoops" |> ReasonReact.string}
      </Heading>
      <p className=Styles.message> {error##message |> ReasonReact.string} </p>
    </div>,
};