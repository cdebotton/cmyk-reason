module Styles = {
  let layout =
    Css.(style([display(grid), gridTemplateColumns([1. |. fr, 1. |. fr])]));

  let userItem =
    Css.(
      style([
        display(`flex),
        alignItems(center),
        display(block),
        padding(0.5 |. rem),
        hover([backgroundColor(hsla(200, 50, 50, 0.5))]),
      ])
    );

  let avatar =
    Css.(
      style([
        width(3. |. rem),
        height(3. |. rem),
        borderRadius(50. |. pct),
        backgroundColor(black),
        marginRight(1. |. rem),
      ])
    );

  let activeUserItem = Css.(style([textDecoration(underline)]));
};

module Users = [%graphql
  {|
  query Users {
    users {
      id
      email
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
      ...(
           ({result}) =>
             switch (result) {
             | Loading => <Loader />
             | Error(error) => <ApolloError error />
             | Data(response) =>
               <div className=Styles.layout>
                 <div>
                   <Heading level=2> ("Users" |> ReasonReact.string) </Heading>
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
                           ...<Fragment>
                                <div className=Styles.avatar />
                                (user##email |> ReasonReact.string)
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
                          | ["admin", "users", userId] => <AdminUser userId />
                          | _ => ReasonReact.null
                          }
                      )
                 </Router.Consumer>
               </div>
             }
         )
    </UsersQuery>,
};