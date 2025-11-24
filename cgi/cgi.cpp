/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agorski <agorski@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:33:44 by agorski           #+#    #+#             */
/*   Updated: 2025/11/24 17:36:43 by agorski          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

std::string executeCgi(const std::string& scriptPath,
                              const std::string& interpreterPath,
                              const std::string& body, 
                              std::map <std::string, std::string> envVars)
{
    int pipeStout[2];
    int pipeStin[2];
    pid_t pid;

    try
    {
    if (pipe(pipeStout) == -1 || pipe(pipeStin) == -1)
        throw std::runtime_error("Pipe Error");

    pid = fork();
    
    if (pid < 0)
        {
        close(pipeStout[0]);
        close(pipeStout[1]);
        close(pipeStin[0]);
        close(pipeStin[1]);
        throw std::runtime_error("Fork Error");
        }

    if (pid == 0) // Child process
    {
        // close unused pipe ends
        close(pipeStout[1]);
        close(pipeStin[0]);

        // redirect stdin from pipe (body to CGI)
        dup2(pipeStin[1], STDIN_FILENO);
        close(pipeStin[1]);

        // redirect stdout to pipe (CGI output to parent)
        dup2(pipeStout[0], STDOUT_FILENO);
        close(pipeStout[0]);

        // set environment variables
        for (std::map<std::string, std::string>::iterator it = envVars.begin(); it != envVars.end(); ++it)
        {
            setenv(it->first.c_str(), it->second.c_str(), 1);
        }

        // Execute CGI interpreter
        char* args[3];
        args[0] = const_cast<char*>(interpreterPath.c_str());
        args[1] = const_cast<char*>(scriptPath.c_str());
        args[2] = NULL;

        if (execve(interpreterPath.c_str(), args, environ) == -1)
            throw std::runtime_error("Execve Error");

        else // Parent process
        {
            close(pipeStout[0]);
            close(pipeStin[1]);
            
            // write body to CGI stdin
            if (!body.empty())
                write(pipeStin[0], body.c_str(), body.size());
            close(pipeStin[0]);

            // read CGI output from stdout
            
            std::string output;
            char buffer[4096];
            ssize_t bytesRead;

            while ((bytesRead = read(pipeStout[1], buffer, sizeof(buffer))) > 0)
            {
                output.append(buffer, bytesRead);
            }

            close(pipeStout[1]);

            // wait for child process to finish
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
                throw std::runtime_error("CGI Execution Failed");
            return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + output;
        }
    }
}
    catch (const std::exception& e)
    {
        // error handling
        return "HTTP/1.1 500 Internal Server Error\r\n\r\n500 " + std::string(e.what());
    }
    return "";
}