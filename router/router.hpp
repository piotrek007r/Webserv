/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agorski <agorski@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 19:55:58 by agorski           #+#    #+#             */
/*   Updated: 2025/11/22 19:57:38 by agorski          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ROUTER_HPP
# define ROUTER_HPP

#include <string>

class Router {
    
private:
    // Add private members if needed
    

public:
    Router();
    ~Router();
    std::string routeRequest(const std::string& requestPath);
};

#endif // ROUTER_HPP