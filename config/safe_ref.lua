-- From https://gist.github.com/eliasdaler/f3516d3deabc32b465a7c244ff082cf0
--

Handles = {}
local memoizedFuncs = {}

function validate(handle)
    if not handle.isValid then
        print(debug.traceback())
        error("[!] Invalid handle!", 2)
    end
end


local mt = {}
mt.__index = function(key, handle)
    validate(handle)

    local f = memoizedFuncs[key]
    if not f then
        f = function(handle, ...) return Entity[key](handle.cppRef, ...) end
    end
    return f
end

function getWrappedSafeFunction(f)
    return function(handle, ...)
        validate(handle)
        return f(handle.cppRef, ...)
    end
end

function createHandle(cppRef)
    local handle = {
        cppRef = cppRef,
        isValid = true
    }

    handle.getName = getWrappedSafeFunction(Entity.getName)

    setmetatable(handle, mt)
    Handles[cppRef:getId()] = handle
    return handle
end

function onEntityRemoved(cppRef)
    local handle = Handles[cppRef:getId()]
    handle.cppRef = nil
    handle.isValid = false
    Handles[cppRef:getId()] = nil
end

function addEntity()
    local ref = createEntity()
    return createHandle(ref)
end

local player = addEntity()
print(player:getId())
