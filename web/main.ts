type UserData = {
	role: "send" | "receive";
};

type User = {
	id: string;
	outgoingMessages: string[];
	data: UserData;
	send: (message: string) => void;
};

const users: User[] = [];

Deno.serve((req: Request) => {
	if (req.headers.get("upgrade") != "websocket") {
		const path = new URL(req.url).pathname;

		switch (path) {
			case "/":
				return new Response("Hello, world!", {
					headers: {
						"Content-Type": "text/plain",
					},
				});

			default:
				try {
					const file = Deno.openSync("./client" + path, {read: true});
					const readableStream = file.readable;

					return new Response(readableStream);
				} catch {
					return new Response("404 Not Found", {
						status: 404,
						headers: {
							"Content-Type": "text/plain",
						},
					});
				}
		}
	}

	const {socket, response} = Deno.upgradeWebSocket(req);

	socket.addEventListener("open", (_req: Event) => {
		let id: string = "";

		while (id.length <= 0 || users.find((user) => user.id === id)) {
			id = Math.random().toString(36).substring(7);
		}

		const self: User = {
			id,
			outgoingMessages: [],
			data: {
				role: "receive",
			},
			send: (message: string) => self.outgoingMessages.push(message),
		};

		users.push(self);

		socket.send(id);

		(async () => {
			while (true) {
				for (const message of self.outgoingMessages) {
					socket.send(message);
					self.outgoingMessages.splice(self.outgoingMessages.indexOf(message), 1);
				}

				await new Promise((resolve) => setTimeout(resolve, 10));
			}
		})();

		socket.addEventListener("close", () => {
			users.splice(users.indexOf(self), 1);
		});

		socket.addEventListener("message", (req: MessageEvent) => {
			console.log(req.data);
		});
	});

	return response;
});
