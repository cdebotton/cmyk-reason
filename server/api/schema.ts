import { makeExecutableSchema, IResolvers } from 'graphql-tools';
import { importSchema } from 'graphql-import';
import { Prisma } from 'prisma-binding';

const typeDefs = importSchema('server/api/schema.graphql');

interface Context {
  db: Prisma;
}

const resolvers: IResolvers<{}, Context> = {
  Query: {
    users: (_parent, _args, context, info) => context.db.query.users({}, info),
    user: (_parent, args, context, info) => {
      return context.db.query.user(args, info);
    },
    session: (_parent, _args, context, info) => {
      return null;
    },
  },
};

const schema = makeExecutableSchema({
  typeDefs,
  resolvers,
});

export default schema;
