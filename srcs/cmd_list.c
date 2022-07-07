#include "../minishell.h"

t_cmd	*cmd_new(t_list *tokens, t_redir *redir)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	new->next = NULL;
	new->tokens = tokens;
	new->redir = redir;
}
