module User = [%graphql
  {|
  query User($where: UserWhereUniqueInput!) {
    user(where: $where) {
      id
      email
    }
  }
|}
];

module UserQuery = ReasonApollo.CreateQuery(User);

module FormConfig = {
  type value = string;
  type t = {email: string};
  type key =
    | Email;
  let debugName = "UserForm";
  let get = (key, state) =>
    switch (key) {
    | Email => state.email
    };
  let set = ((key, value), _state) =>
    switch (key) {
    | Email => {email: value}
    };
};
module UserForm = Form.Make(FormConfig);

let onSubmit = values => Js.log(values);

let component = ReasonReact.statelessComponent("AdminUser");

let make = (~userId, _children) => {
  let userQuery = User.make(~where={"id": Some(userId), "email": None}, ());
  {
    ...component,
    render: _self =>
      <div>
        <UserQuery variables=userQuery##variables>
          ...(
               ({result}) =>
                 switch (result) {
                 | Loading => <Loader />
                 | Error(error) => <ApolloError error />
                 | Data(response) =>
                   <div>
                     <p> (response##user##id |> ReasonReact.string) </p>
                     <UserForm
                       initialValues={email: response##user##email} onSubmit>
                       ...(
                            ({onChange, getValue}) =>
                              <form>
                                <label>
                                  ("Email" |> ReasonReact.string)
                                </label>
                                <input
                                  value=(getValue(FormConfig.Email))
                                  onChange=(
                                    event => {
                                      let obj =
                                        event
                                        |> ReactEventRe.Form.target
                                        |> ReactDOMRe.domElementToObj;
                                      obj##value |> onChange(Email);
                                    }
                                  )
                                />
                              </form>
                          )
                     </UserForm>
                   </div>
                 }
             )
        </UserQuery>
      </div>,
  };
};
1;