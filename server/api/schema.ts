import { makeExecutableSchema, IResolvers } from 'graphql-tools';
import { importSchema } from 'graphql-import';
import { Prisma } from 'prisma-binding';
import jwt from 'jsonwebtoken';
import bcrypt from 'bcryptjs';

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
  Mutation: {
    authenticate: async (_parent, args, context, _info) => {
      const user = await context.db.query.user({
        where: { email: args.input.email },
      });

      if (!user) {
        throw new Error('Invalid');
      }

      const match = await bcrypt.compare(args.input.password, user.password);

      if (!match) {
        throw new Error('Invalid');
      }

      const token = jwt.sign({ user: user.id }, 'SECRETS');

      return token;
    },
  },
};

const schema = makeExecutableSchema({
  typeDefs,
  resolvers,
});

export default schema;
