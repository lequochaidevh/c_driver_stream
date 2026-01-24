-- vim.notify = function(msg, level, opts)
--   if msg:match("nvim%-lspconfig support for Nvim") then
--     return
--   end
--   return vim.api.nvim_notify(msg, level, opts)
-- end

return {
  "hrsh7th/nvim-cmp",
  dependencies = {
    "hrsh7th/cmp-nvim-lsp",
    "L3MON4D3/LuaSnip",
  },
  config = function()
    local cmp = require("cmp")

    cmp.event:on("menu_opened", function()
      vim.schedule(function()
        if cmp.visible() then
          cmp.select_next_item({ behavior = cmp.SelectBehavior.Select })
        end
      end)
    end)

    cmp.setup({
      preselect = cmp.PreselectMode.Item,

      completion = {
        autocomplete = { cmp.TriggerEvent.TextChanged },
      },

      mapping = cmp.mapping.preset.insert({
        ["<C-Space>"] = cmp.mapping.complete(),

        -- Enter: giữ nguyên hành vi mặc định (xuống dòng)
        ["<CR>"] = cmp.mapping(function(fallback)
          fallback()
        end, { "i", "s" }),

        -- TAB = confirm
        ["<Tab>"] = cmp.mapping(function(fallback)
          if cmp.visible() then
            cmp.confirm({ select = true })
          else
            fallback() -- indent / tab bình thường
          end
        end, { "i", "s" }),

        -- Alt navigation
        ["<M-j>"] = cmp.mapping.select_next_item(),
        ["<M-k>"] = cmp.mapping.select_prev_item(),

        ["<S-Tab>"] = cmp.mapping(function(fallback)
          if cmp.visible() then
            cmp.select_prev_item()
          else
            fallback()
          end
        end, { "i", "s" }),
      }),

      sources = {
        { name = "nvim_lsp" },
      },
    })
  end,
}