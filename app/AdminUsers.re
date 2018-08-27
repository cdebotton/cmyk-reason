module Styles = {
  open Css;
  let layout =
    style([
      display(grid),
      gridTemplateColumns([1. |> fr, 1. |> fr]),
      gridGap(1. |> rem),
    ]);

  let userItem =
    style([
      display(`flex),
      alignItems(center),
      display(block),
      padding(1. |> rem),
      borderBottom(1 |> px, solid, hsla(200, 25, 25, 0.25)),
      hover([backgroundColor(hsla(200, 25, 25, 0.05))]),
    ]);

  let avatar =
    style([
      width(3. |> rem),
      height(3. |> rem),
      borderRadius(50. |> pct),
      backgroundColor(black),
      marginRight(1. |> rem),
    ]);

  let badge = style([marginLeft(auto)]);
  let activeUserItem = style([textDecoration(underline)]);
};

module Users = [%graphql
  {|
  query Users {
    users {
      id
      email
      role
    }
  }
|}
];

module UsersQuery = ReasonApollo.CreateQuery(Users);

let component = ReasonReact.statelessComponent("AdminUsers");

let make = _children => {
  ...component,
  render: _self =>
    <UsersQuery>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => <Loader />
             | Error(error) => <ApolloError error />
             | Data(response) =>
               <div className=Styles.layout>
                 <div>
                   <Heading level=2> {"Users" |> ReasonReact.string} </Heading>
                   <Popover
                     size=(400, 300)
                     title="New user"
                     label="Create a new user">
                     ...(
                          ({close}) =>
                            <AdminCreateUser onSave=close onCancel=close />
                        )
                   </Popover>
                   <ItemList
                     items=response##users
                     getKey=(
                       user => {
                         let id = user##id;
                         {j|USER_$id|j};
                       }
                     )
                     renderItem=(
                       user => {
                         let id = user##id;
                         let href = {j|/admin/users/$id|j};
                         <Link
                           href
                           className=Styles.userItem
                           activeClassName=Styles.activeUserItem>
                           <Fragment>
                             <div className=Styles.avatar />
                             {user##email |> ReasonReact.string}
                             <Badge
                               className=Styles.badge
                               format={
                                 switch (user##role) {
                                 | `UNAUTHORIZED => Negative
                                 | _ => Neutral
                                 }
                               }>
                               {
                                 user##role
                                 |> Role.roleToString
                                 |> ReasonReact.string
                               }
                             </Badge>
                           </Fragment>
                         </Link>;
                       }
                     )
                   />
                 </div>
                 <Router.Consumer>
                   ...(
                        ({path}) =>
                          switch (path) {
                          | ["admin", "users", userId] =>
                            <AdminUpdateUser userId />
                          | _ => ReasonReact.null
                          }
                      )
                 </Router.Consumer>
               </div>
             }
         }
    </UsersQuery>,
};