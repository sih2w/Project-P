--!strict
local CollectionService = game:GetService("CollectionService")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local ServerScriptService = game:GetService("ServerScriptService")

local SharedClasses = ReplicatedStorage.SharedClasses
local Maid = require(SharedClasses.Default.Maid)

type Edge = {
	FromVertexId: number,
	ToVertexId: number	
}

type Tree = {
	Edges: {Edge},
	StartVertexId: number,
	EndVertexId: number,
}

type Worker = {
	Seed: number,
	EdgeCount: number,
	StartVertexId: number,
	EndVertexId: number,
	Directed: boolean,
	Index: number,
	WorkerActor: Actor,
	MasterActor: Actor
}

type Params = {
	VertexCount: number,
	EdgeCount: number,
	Seed: number,
	WorkerCount: number,
	Directed: boolean
}

local function getNeighbors(vertexId: number, tree: Tree)
	local neighbors: {number} = {}

	for _, edge in tree.Edges do
		local neighbor: number = nil
		
		if edge.ToVertexId == vertexId then
			neighbor = edge.FromVertexId
		elseif edge.FromVertexId == vertexId then
			neighbor = edge.ToVertexId
		end
		
		if table.find(neighbors, neighbor) == nil then
			table.insert(neighbors, neighbor)
		end
	end

	return neighbors
end

local function printTree(tree: Tree, vertexCount: number)
	for vertexId = 1, vertexCount do
		local output = `{vertexId}`

		local neigbors = getNeighbors(vertexId, tree)
		for _, vertexId in neigbors do
			output = `{output} -> {vertexId}`
		end

		print(output)
	end
end

local function drawTree(tree: Tree, vertexCount: number)
	local vertices: {BasePart} = {}
	local random = Random.new(0)
	
	for vertexId = 1, vertexCount do
		local scalar = 10 + random:NextNumber() * vertexCount * 5
		local position = random:NextUnitVector() * scalar
		
		local part = Instance.new("Part")
		part.Name = `Vertex_{vertexId}`
		part.CanCollide = false
		part.CanQuery = false
		part.CastShadow = false
		part.Anchored = true
		part.Shape = Enum.PartType.Ball
		part.Material = Enum.Material.SmoothPlastic
		part.Size = Vector3.one * 5
		part.PivotOffset = CFrame.new(0, -part.Size.Y / 2, 0)
		part.Color = Color3.fromHSV(random:NextNumber(), 1, 1)
		part:PivotTo(CFrame.new(position))
		part:SetAttribute("VertexId", vertexId)
		part.Parent = workspace
		CollectionService:AddTag(part, "Vertex")
		
		table.insert(vertices, part)
	end
	
	for vertexId = 1, vertexCount do
		local neigbors = getNeighbors(vertexId, tree)
		for _, neighbor in neigbors do
			local vertex1 = vertices[vertexId]
			local vertex2 = vertices[neighbor]
			
			local attachment0 = vertex1:FindFirstChildWhichIsA("Attachment") or Instance.new("Attachment", vertex1)
			local attachment1 = vertex2:FindFirstChildWhichIsA("Attachment") or Instance.new("Attachment", vertex2)
			local beam = Instance.new("Beam")
			beam.Attachment0 = attachment0
			beam.Attachment1 = attachment1
			beam.FaceCamera = true
			beam.Color = ColorSequence.new(vertex1.Color, vertex2.Color)
			beam:SetAttribute("Vertex1Id", vertexId)
			beam:SetAttribute("Vertex2Id", neighbor)
			beam.Parent = vertex1
			
			CollectionService:AddTag(beam, "Edge")
		end
	end
end

local function combineTrees(trees: {Tree}, vertexCount: number)
	local bigTree: Tree = {
		Edges = {},
		StartVertexId = 1,
		EndVertexId = vertexCount
	}

	for _, smallTree in trees do
		for _, edge in smallTree.Edges do
			table.insert(bigTree.Edges, edge)
		end
	end

	for index = 2, #trees do
		table.insert(bigTree.Edges, {
			FromVertexId = trees[index - 1].EndVertexId,
			ToVertexId = trees[index].StartVertexId
		})
	end
	
	return bigTree
end

local function createWorkers(params: Params)
	local verticesPerWorker = params.VertexCount // params.WorkerCount
	local edgesPerWorker = params.EdgeCount // params.WorkerCount

	local workers: {Worker} = {}

	for index = 1, params.WorkerCount do
		local worker: Worker = {
			StartVertexId = ((index - 1) * verticesPerWorker) + 1,
			EndVertexId = index * verticesPerWorker,
			EdgeCount = edgesPerWorker,
			Seed = params.Seed,
			Index = index,
			Directed = params.Directed,
			MasterActor = script.Parent,
			WorkerActor = Instance.new("Actor")
		}

		if index == params.WorkerCount then
			worker.EndVertexId += math.fmod(params.VertexCount, params.WorkerCount)
			worker.EdgeCount += math.fmod(params.EdgeCount, params.WorkerCount)
		end

		table.insert(workers, worker)
	end
	
	return workers
end

local function connected(startVertexId: number, tree: Tree)
	local visited = {}
	local queue = {startVertexId}

	while #queue > 0 do
		local current = table.remove(queue, 1)
		if not visited[current] then
			visited[current] = true
			for _, edge in tree.Edges do
				if edge.FromVertexId == current and not visited[edge.ToVertexId] then
					table.insert(queue, edge.ToVertexId)
				elseif edge.ToVertexId == current and not visited[edge.FromVertexId] then
					table.insert(queue, edge.FromVertexId)
				end
			end
		end
	end

	for _, edge in tree.Edges do
		if not visited[edge.FromVertexId] or not visited[edge.ToVertexId] then
			return false
		end
	end

	return true
end

local function isEdgeValid(edge1: Edge, tree: Tree, directed: boolean)
	if edge1.ToVertexId == edge1.FromVertexId then
		return false
	end

	for _, edge2 in tree.Edges do
		local exists = edge1.ToVertexId == edge2.ToVertexId and edge1.FromVertexId == edge2.FromVertexId
		if exists then
			return false
		end

		if directed then
			local backwards = edge1.ToVertexId == edge2.FromVertexId and edge1.FromVertexId == edge2.ToVertexId
			if backwards then
				return false
			end
		end
	end

	return true
end

local function createEdge(vertexId: number, tree: Tree, directed: boolean, random: Random)
	local attempt = 0
	
	while attempt < 100 do
		local edge: Edge = {
			ToVertexId = vertexId,
			FromVertexId = random:NextInteger(tree.StartVertexId, tree.EndVertexId)
		}

		if isEdgeValid(edge, tree, directed) then
			table.insert(tree.Edges, edge)
			return true
		else
			attempt += 1
		end
	end

	return false
end

local function createTree(worker: Worker)
	local tree: Tree = {
		Edges = {},
		StartVertexId = worker.StartVertexId,
		EndVertexId = worker.EndVertexId
	}
	
	local edgesRemaining = worker.EdgeCount
	local random = Random.new(worker.Index + worker.Seed)
	
	local finished = false
	local attempt = 0

	while not finished do
		for vertexId = worker.StartVertexId, worker.EndVertexId do
			local success = createEdge(vertexId, tree, worker.Directed, random)
			if success then
				edgesRemaining -= 1
			end
		end

		local connected = connected(worker.StartVertexId, tree)
		if connected then
			if edgesRemaining > 0 then
				attempt += 1
			end
		end

		finished = connected and (attempt > 10 or edgesRemaining <= 0)
	end
	
	return tree
end

local function getPath(startVertexId: number, endVertexId: number, tree: Tree): {number}?
	local queue: {{Vertex: number, Path: {number}}} = {{Vertex = startVertexId, Path = {startVertexId}}}
	local visited = {[startVertexId] = true}

	-- BFS to find the shortest path
	while #queue > 0 do
		local current = table.remove(queue, 1)  -- Dequeue
		if current == nil then
			return nil
		end
		
		-- Check if we reached the destination
		if current.Vertex == endVertexId then
			return current.Path  -- Return the path to the destination
		end

		-- Explore neighbors
		for _, edge in tree.Edges do
			local neighbor
			if edge.FromVertexId == current.Vertex then
				neighbor = edge.ToVertexId
			elseif edge.ToVertexId == current.Vertex then
				neighbor = edge.FromVertexId
			end

			if neighbor and not visited[neighbor] then
				visited[neighbor] = true
				local newPath = {table.unpack(current.Path)}  -- Create a copy of the path
				table.insert(newPath, neighbor)  -- Add the neighbor to the path
				table.insert(queue, {Vertex = neighbor, Path = newPath})  -- Enqueue the neighbor
			end
		end
	end

	return nil  -- Return nil if no path is found
end

local function drawPath(path: {number})
	for _, instance in CollectionService:GetTagged("Vertex") do
		local vertex: BasePart = instance
		local vertexId = vertex:GetAttribute("VertexId")
		if vertexId == path[1] then
			vertex.Color = Color3.fromRGB(85, 255, 0)
		elseif vertexId == path[#path] then
			vertex.Color = Color3.fromRGB(255, 0, 0)
		else
			vertex.Color = Color3.fromRGB(255, 255, 255)
		end
	end
	
	for _, instance in CollectionService:GetTagged("Edge") do
		local beam: Beam = instance
		local success = table.find(path, beam:GetAttribute("Vertex1Id"))
		success = success and table.find(path, beam:GetAttribute("Vertex2Id"))
		beam.Color = ColorSequence.new(
			if success then Color3.fromRGB(170, 0, 255) else Color3.fromRGB(255, 255, 255)
		)
	end
end

local function tryDrawPath(vertexCount: number, tree: Tree)
	local path = getPath(
		math.max(1, script:GetAttribute("Start")),
		math.min(vertexCount, script:GetAttribute("Target")),
		tree
	)
	
	if path then
		drawPath(path)
	end
end

do
	local actor = script:GetActor()
	if actor.Name == "Master" then
		local params: Params = {
			VertexCount = 1000,
			EdgeCount = 20000,
			Seed = 0,
			Directed = true,
			WorkerCount = 50
		}

		local workers: {Worker} = createWorkers(params)
		for _, worker in workers do
			script:Clone().Parent = worker.WorkerActor
			worker.WorkerActor.Parent = ServerScriptService
		end
		
		local start = os.clock()
		local trees: {Tree} = {}
		
		actor:BindToMessageParallel("TreeCompleted", function(tree: Tree)
			table.insert(trees, tree)
			
			if #trees == #workers then
				local tree = combineTrees(trees, params.VertexCount)
				local elapsed = os.clock() - start
				print(`Time to create tree: {elapsed}`)
				
				task.synchronize()
				
				drawTree(tree, params.VertexCount)
				
				actor:BindToMessageParallel("PathFound", function(tree: Tree)
					
				end)

				script:GetAttributeChangedSignal("Start"):Connect(function()
					tryDrawPath(params.VertexCount, tree)
				end)

				script:GetAttributeChangedSignal("Target"):Connect(function()
					tryDrawPath(params.VertexCount, tree)
				end)
			end
		end)
		
		task.defer(function()
			for index, worker in workers do
				worker.WorkerActor:SendMessage("CreateTree", worker)
			end
		end)
	else
		actor:BindToMessageParallel("CreateTree", function(worker: Worker)
			local tree = createTree(worker)
			worker.MasterActor:SendMessage("TreeCompleted", tree)
		end)
		
		local maid = Maid.new()
		
		actor:BindToMessageParallel("CancelPath", function()
			maid:Clean()
		end)
		
		actor:BindToMessageParallel("FindPath", function(tree: Tree, index: number, vertexCount: number)
		
		end)
	end
end
