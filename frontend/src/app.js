import { BrowserRouter as Router, Switch, Route } from 'react-router-dom';

import Home from './pages/home';
import Search from './pages/search';

const App = () => (
  <div className="app">
    <Router>
      <Switch>
        <Route path="/search">
          <Search />
        </Route>
        <Route path="/">
          <Home />
        </Route>
      </Switch>
    </Router>
  </div>
);

export default App;
