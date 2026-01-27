return {
  {
    "neovim/nvim-lspconfig",
    version = "v0.1.7", -- ok cho nvim 0.9.x
    dependencies = {
      "hrsh7th/cmp-nvim-lsp",
    },
    config = function()
      local lspconfig = require("lspconfig")
      local capabilities = require("cmp_nvim_lsp").default_capabilities()

      -- clangd
      lspconfig.clangd.setup({
        cmd = {
          "clangd",
          "--background-index",
          "--clang-tidy",
          "--completion-style=detailed",
          "--header-insertion=iwyu",
          "--pch-storage=memory",
        },
        filetypes = { "c", "cpp", "objc", "objcpp" },
        root_dir = lspconfig.util.root_pattern(
          "compile_commands.json",
          ".clangd",
          "CMakeLists.txt",
          ".git"
        ),
        capabilities = capabilities,
      })

      -- ✅ FORMAT ON SAVE (PHẢI nằm trong config)
      vim.api.nvim_create_autocmd("BufWritePre", {
        pattern = { "*.c", "*.cpp", "*.h", "*.hpp" },
        callback = function()
          vim.lsp.buf.format({
            async = false,
            timeout_ms = 2000,
          })
        end,
      })
    end,
  },
}

