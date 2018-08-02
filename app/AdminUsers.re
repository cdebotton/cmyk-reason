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
               <Fragment>
                 <ul>
                   (
                     Belt.Array.map(
                       response##users,
                       user => {
                         let id = user##id;
                         <li key=user##id>
                           <Link href={j|/admin/users/$id|j}>
                             (user##email |> ReasonReact.string)
                           </Link>
                         </li>;
                       },
                     )
                     |> ReasonReact.array
                   )
                 </ul>
                 <Router.Consumer>
                   ...(
                        ({path}) =>
                          switch (path) {
                          | ["admin", "users", userId] => <AdminUser userId />
                          | _ => ReasonReact.null
                          }
                      )
                 </Router.Consumer>
               </Fragment>
             }
         )
    </UsersQuery>,
};