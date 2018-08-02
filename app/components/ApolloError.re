let component = ReasonReact.statelessComponent("ApolloError");

let make = (~error: ReasonApolloTypes.apolloError, _children) => {
  ...component,
  render: _self => <p> (error##message |> ReasonReact.string) </p>,
};