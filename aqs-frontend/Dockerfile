# Билдим фронт
FROM node:22-alpine AS builder
WORKDIR /app
COPY . .
RUN npm install && npm run build

# Сервер — Nginx
FROM nginx:stable-alpine
COPY --from=builder /app/dist /usr/share/nginx/html

# Настройка Nginx
COPY nginx.conf /etc/nginx/conf.d/default.conf

EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
