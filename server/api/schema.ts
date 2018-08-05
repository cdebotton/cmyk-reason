import { makeExecutableSchema, IResolvers } from 'graphql-tools';
import { importSchema } from 'graphql-import';
import { Prisma } from 'prisma-binding';
import jwt from 'jsonwebtoken';
import bcrypt from 'bcryptjs';

const typeDefs = importSchema('server/api/schema.graphql');

interface Context {
  db: Prisma;
  getToken: () => string | null;
  setToken: (token: string | undefined) => void;
}

const resolvers: IResolvers<{}, Context> = {
  Query: {
    users: (_parent, args, context, info) => context.db.query.users(args, info),
    user: (_parent, args, context, info) => {
      return context.db.query.user(args, info);
    },
    session: async (_parent, _args, context, info) => {
      const token = context.getToken();

      if (!token) {
        return null;
      }

      const decoded = jwt.decode(token);

      if (!decoded || typeof decoded === 'string') {
        return null;
      }

      return {
        token,
        userId: decoded.user,
      };
    },
  },
  Session: {
    user: (parent: any, _args, context, info) => {
      return context.db.query.user({ where: { id: parent.userId } }, info);
    },
  },
  Mutation: {
    createUser: async (_parent, args, context, info) => {
      const salt = await bcrypt.genSalt(10);
      const password = await bcrypt.hash(args.data.password, salt);

      return await context.db.mutation.createUser(
        {
          data: {
            ...args.data,
            password,
          },
        },
        info,
      );
    },
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

      context.setToken(token);

      return token;
    },
    logout: (_parent, _args, context, _info) => {
      const token = context.getToken();

      if (!token) {
        return null;
      }

      context.setToken(undefined);

      return token;
    },
  },
};

const schema = makeExecutableSchema({
  typeDefs,
  resolvers,
});

export default schema;
