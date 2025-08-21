/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:50:28 by rpaulo-c          #+#    #+#             */
/*   Updated: 2025/08/21 20:52:17 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

typedef enum e_token_type
{
	E_TOKEN_WORD,			// Para palavras comuns, argumentos, comandos, etc.
	E_TOKEN_PIPE,			// Para o metacaractere '|'
	E_TOKEN_REDIR_IN,		// Para o metacaractere '<'
	E_TOKEN_REDIR_OUT,		// Para o metacaractere '>'

	E_TOKEN_REDIR_APPEND,	// '>>'
	E_TOKEN_HEREDOC,		// '<<'
	E_TOKEN_EOF				// Um token especial para indicar o fim da entrada
} t_token_type;

typedef struct s_token
{
	char			*value;	// O conteúdo da string do token (alocado com gc_malloc)
	t_token_type	type;	// O tipo de token (ex: E_TOKEN_WORD, E_TOKEN_PIPE)
	struct s_token	*next;	// Ponteiro para o próximo token na lista encadeada
} t_token;
