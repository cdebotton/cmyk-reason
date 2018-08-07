let roleToString = role =>
  switch (role) {
  | `ADMIN => "admin"
  | `USER => "user"
  | `UNAUTHORIZED => "unauthorized"
  | `EDITOR => "editor"
  };

let stringToRole = role =>
  switch (role) {
  | "admin" => `ADMIN
  | "user" => `USER
  | "unauthorized" => `UNAUTHORIZED
  | "editor" => `EDITOR
  | role => Js.Exn.raiseError({j|Unrecognized role $role|j})
  };